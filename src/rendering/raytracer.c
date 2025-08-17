/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/07/30 07:37:53 by cuistobal        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"
/*
** setup_camera - Initialize camera coordinate system
*/
void	setup_camera(t_camera *camera)
{
	t_vec3	world_up;

	if (!camera)
		return ;
	camera->direction = vec3_normalize(camera->direction);
	world_up = vec3_new(0, 1, 0);
	camera->right = vec3_normalize(vec3_cross(camera->direction, world_up));
	camera->up = vec3_normalize(vec3_cross(camera->right, camera->direction));
}

/*
** get_camera_ray - Generate ray from camera through pixel coordinates
*/
t_ray	get_camera_ray(t_minirt *rt, t_camera *camera, double u, double v)
{
	t_vec3	pixel_world;
	t_vec3	ray_direction;
	double	half_height;
	double	half_width;

	if (!camera)
		return (ray_new(vec3_new(0, 0, 0), vec3_new(0, 0, 1)));
	calculate_viewport_dimensions(rt, camera, &half_width, &half_height);
	pixel_world = calculate_pixel_world_position(camera, u, v,
			half_width, half_height);
	ray_direction = vec3_sub(pixel_world, camera->position);
	return (ray_new(camera->position, ray_direction));
}

// inter thread

//t_intels directives_rendering[3600];

/*
** render_all_pixels - Render all pixels in the scene
*/
static void send_directive(t_intels directives)
{
	pthread_mutex_lock(&(directives.rt->args.mutexQueue));
    directives.rt->args.directives_rendering[directives.rt->args.ntask] = directives;
    directives.rt->args.ntask++;
    pthread_mutex_unlock(&(directives.rt->args.mutexQueue));
    pthread_cond_signal(&(directives.rt->args.condQueue)); // 3.1 Ask pthread_cond_wait to check its condition
}
void create_directive(t_minirt *rt)
{
	int pwidth = ((rt->mlx.width + 31) / 32); // init in rt
	int pheight = ((rt->mlx.height + 31) / 32); // init in rt
	int i = 0;
	int y;
	t_intels directives;

	(rt->args.total_directives) = pwidth * pheight;
	while (i < pheight)
	{
		y = 0;
		while (y < pwidth)
		{
			directives.xstart = y * 32;
			if (y * 32 + 32 < rt->mlx.width)
				directives.xend = y * 32 + 32;
			else 
				directives.xend = rt->mlx.width;
			directives.ystart = i * 32;
			if (i * 32 + 32 < rt->mlx.height)
				directives.yend = i * 32 + 32;
			else
				directives.yend = rt->mlx.height;
			directives.rt = rt;
			send_directive(directives); // 3. Send directives
			y++;
		}
		i++;
	}
}
static void execute_rendering(t_intels* task)
{
	int		x;
	int		y;
	double	inv_width;
	double	inv_height;

	inv_width = 1.0 / (double)task->rt->mlx.width;
	inv_height = 1.0 / (double)task->rt->mlx.height;
	y = task->ystart;
	while (y < task->yend) 
	{
		x = task->xstart;
		while (x < task->xend)
		{
			render_pixel_at_coordinates(task->rt, x, y, inv_width, inv_height);
			x++;
		}
		y++;
	}	
}

void	*render_all_pixels(void *arg)
{
	t_minirt *rt;
	t_intels directive;
	int i;
	
	rt = (t_minirt *)arg;
	while (1)
	{
		i = 0;
		pthread_mutex_lock(&(rt->args.mutexQueue));
		while (rt->args.ntask == 0 && rt->args.stop == 0)
		{
			pthread_cond_wait(&rt->args.condQueue, &(rt->args.mutexQueue)); // 1.1 wait until his condition change. Wait for 
		}
		if (rt->args.stop == 1 && rt->args.ntask == 0)
		{
			pthread_mutex_unlock(&(rt->args.mutexQueue));
			break;
		}
		directive = rt->args.directives_rendering[0];
		while (i < rt->args.ntask - 1)
		{
			rt->args.directives_rendering[i] = rt->args.directives_rendering[i + 1];
			i++;
		}
		rt->args.ntask--;
		pthread_mutex_unlock(&(rt->args.mutexQueue));
		execute_rendering(&directive);
		pthread_mutex_lock(&(rt->args.mutexQueue));
		rt->args.completed_directives++;
		if (rt->args.completed_directives == (rt->args.total_directives))
		{
			rt->args.stop = 1;
			pthread_cond_broadcast(&(rt->args.condQueue));
		}
		pthread_mutex_unlock(&(rt->args.mutexQueue));
	}
	rt->args.completed_directives = 0;
	return NULL;
}



/*
** render_scene - Main rendering function
*/


void	render_scene(t_minirt *rt)
{
	pthread_t threads[NUM_THREAD];
	int i;

	i = 0;
	if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
		return ;
	rt->args.stop = 0;
	rt->args.ntask = 0;
	rt->args.completed_directives = 0;
	setup_camera(&rt->scene.camera);
	pthread_mutex_init(&(rt->args.mutexQueue), NULL); // init mutex for queue
	while(i < NUM_THREAD)
	{
		if (pthread_create(&threads[i], NULL, render_all_pixels, rt) != 0) // 1. Thread creation
			perror("Error : Thread creation failde"); // manage error
		i++;
	}
	create_directive(rt); // 2. Create directivre
	i = 0;
	while (i < NUM_THREAD)
	{
		pthread_join(threads[i], NULL); // End all threads
		i++;
	}
	display_image(&rt->mlx);
}

/*
** raycast - Main ray casting function
*/
t_color	raycast(t_ray ray, t_scene *scene, int depth)
{
	t_hit	hit;

	if (!scene || depth <= 0)
		return (color_new(0, 0, 0));
	if (intersect_scene(ray, scene, &hit))
		return (calculate_hit_color(ray, &hit, scene, depth));
	return (calculate_background_color(ray));
}
