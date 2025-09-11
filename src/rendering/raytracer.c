/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/10 17:42:58 by chrleroy         ###   ########.fr       */
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
	double	half_width;
	double	half_height;
	t_vec3	pixel_world;
	t_vec3	ray_direction;

	if (!camera)
		return (ray_new(vec3_new(0, 0, 0), vec3_new(0, 0, 1)));
	calculate_viewport_dimensions(rt, camera, &half_width, &half_height);
	pixel_world = calculate_pixel_world_position(camera, u, v,
			half_width, half_height);
	ray_direction = vec3_sub(pixel_world, camera->position);
	return (ray_new(camera->position, ray_direction));
}

/*
** render_all_pixels - Render all pixels in the scene
*/
static void send_directive(t_intels dirs)
{
	pthread_mutex_lock(&(dirs.rt->args.mutexQueue));
//	if (dirs.rt->args.ntask < MAX_TASKS)
  //  {
		dirs.rt->args.directives_rendering[dirs.rt->args.ntask] = dirs;
		dirs.rt->args.ntask++;
//	}
	pthread_mutex_unlock(&(dirs.rt->args.mutexQueue));
	pthread_cond_signal(&(dirs.rt->args.condQueue));
}

// J ai modifie ton code -> j ai retire les if/else pour reduire le branching, etant donne
// que les valeurs seront toujours valide (la resolution est forcement un multiple de 32)
// J ai utilise << 5 qui equivaut a * 32 (2^5=32) mais en plsu rapide.
// En tout ca nous fait gagner 100 ms
// Dailleurs on devrait garder ces valeur la dans la structure de pool_thread plutoit que 
// de la recalculer 3600 fois par frame.
void create_directive(t_minirt *rt)
{
	int			i;
	int 		y;
	int 		pwidth;
	int 		pheight;
	t_intels	directives;

	i = 0;
	pwidth = ((rt->mlx.width + 31) >> 5);
	pheight = ((rt->mlx.height + 31) >> 5);
	(rt->args.total_directives) = pwidth * pheight;
	while (i < pheight)
	{
		y = 0;
		while (y < pwidth)
		{
			directives.xstart = y << 5;
			directives.xend = ((y << 5) + 32 < rt->mlx.width) ? (y << 5) + 32 : rt->mlx.width;
			directives.ystart = i << 5;
			directives.yend = ((i << 5) + 32 < rt->mlx.height) ? (i << 5) + 32 : rt->mlx.height;
			directives.rt = rt;
			send_directive(directives);
			y++;
		}
		i++;
	}
}

//
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

// The thread's routine to divide redering into smaller chuncks
void	*render_all_pixels(void *arg)
{
	int			i;
	t_minirt	*rt;
	t_intels	directive;
	
	rt = (t_minirt *)arg;
	while (1)
	{
		i = 0;
		pthread_mutex_lock(&(rt->args.mutexQueue));
		while (rt->args.ntask == 0 && rt->args.stop == 0)
			pthread_cond_wait(&rt->args.condQueue, &(rt->args.mutexQueue));
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
	return (NULL);
}



/*
** render_scene - Main rendering function
*/
void	render_scene(t_minirt *rt)
{
	int				i;
	pthread_t		threads[NUM_THREAD];

	i = 0;
	if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
		return ;
	rt->args.stop = 0;
	rt->args.ntask = 0;
	rt->args.completed_directives = 0;
	setup_camera(&rt->scene.camera);
	pthread_mutex_init(&(rt->args.mutexQueue), NULL);
	pthread_cond_init(&(rt->args.condQueue), NULL);
	while(i < NUM_THREAD)
	{
		if (pthread_create(&threads[i], NULL, render_all_pixels, rt))
			perror("Error : Thread creation failed");
		i++;
	}
	create_directive(rt);
	i = 0;
	while (i < NUM_THREAD)
	{
		pthread_join(threads[i], NULL);
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
	if (intersect_bvh_iter(ray, scene->bvh_root, &hit))
		return (calculate_hit_color(ray, &hit, scene, depth));
	return (calculate_background_color(ray));
}
