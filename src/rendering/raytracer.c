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
#include <pthread.h>
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
#define NUM_THREAD 4

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;
int total_directives = 0;
int stop = 0;
int ntask = 0;
int completed_directives = 0;

typedef struct s_intels
{
	t_minirt *rt;
	int xstart;
	int ystart;
	
	int xend;
	int yend;
	
}	t_intels;
t_intels directives_rendering[3600];

/*
** render_all_pixels - Render all pixels in the scene
*/
static void send_directive(t_intels directives)
{
	pthread_mutex_lock(&mutexQueue);
    directives_rendering[ntask] = directives;
    ntask++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}
void create_directive(t_minirt *rt)
{
	int pwidth = ((rt->mlx.width + 31) / 32); // init in rt
	int pheight = ((rt->mlx.height + 31) / 32); // init in rt
	int i = 0;
	int y;
	t_intels directives;
	
	total_directives = pwidth * pheight;
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
			send_directive(directives);
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
	(void)arg;
	t_intels directive;
	int i;
	
	while (1)
	{
		i = 0;

		pthread_mutex_lock(&mutexQueue);
		while (ntask == 0 && stop == 0)
		{
			pthread_cond_wait(&condQueue, &mutexQueue);
		}
		if (stop == 1 && ntask == 0)
		{
			//write(1,"left",4);
			pthread_mutex_unlock(&mutexQueue);
			break;
		}
		directive = directives_rendering[0];
		while (i < ntask - 1)
		{
			directives_rendering[i] = directives_rendering[i + 1];
			i++;
		}
		ntask--;
		pthread_mutex_unlock(&mutexQueue);
		execute_rendering(&directive);
		pthread_mutex_lock(&mutexQueue);
		completed_directives++;
		if (completed_directives == total_directives)
		{
			stop = 1;
			pthread_cond_broadcast(&condQueue);
		}
		//printf("total = %d conplted = %d stop = %d n task = %d\n",total_directives,completed_directives,stop, ntask);
		pthread_mutex_unlock(&mutexQueue);
	}
	completed_directives = 0;
	//write(1,"left",4);
	return NULL;
}



/*
** render_scene - Main rendering function
*/


void	render_scene(t_minirt *rt)
{
	pthread_t threads[NUM_THREAD];
	t_intels intels[4];

	int i;
	i = 0;
	//printf("Render scene\n");

	if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
		return ;
	setup_camera(&rt->scene.camera);
	pthread_mutex_init(&mutexQueue, NULL); // init mutex for queue
	while(i < NUM_THREAD)
	{
		if (pthread_create(&threads[i], NULL, render_all_pixels, &intels[i]) != 0) // 1. Thread creation
			perror("Error : Thread creation failde"); // manage error
		i++;
	}
	//printf("Thread creation ok\n");
	create_directive(rt);
	i = 0;
	while (i < NUM_THREAD)
	{
		pthread_join(threads[i], NULL); // End all threads
		i++;
	}
	//printf("display\n");
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