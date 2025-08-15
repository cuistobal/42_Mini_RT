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
int ntask = 0;

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

void create_directive(t_intels *directives,t_minirt *rt)
{
	//int directives = (rt->mlx.width / 32) * (rt->mlx.height / 32);
	int pwidth = (rt->mlx.width / 32);
	int pheight = (rt->mlx.height / 32);
	int i = 0;
	int y;
	int index_directive;
	while (i < pheight)
	{
		y = 0;
		while (y < pwidth)
		{
			directives_rendering[index_directive].xstart = y * 32;
			directives_rendering[index_directive].xend = y * 32 + 32;
			directives_rendering[index_directive].ystart = i * 32;
			directives_rendering[index_directive].yend = i * 32 + 32;
			index_directive++;
			y++;
		}
		i++;
	}
}
static void execute_rendering(void * task)
{
	t_intels  *intel = (t_intels*) task;

	int		x;
	int		y;
	double	inv_width;
	double	inv_height;

	inv_width = 1.0 / (double)intel->rt->mlx.width;
	inv_height = 1.0 / (double)intel->rt->mlx.height;
	y = intel->ystart;
	while (y < intel->rt->mlx.height) 
	{
		x = intel->xstart;
		while (x < intel->rt->mlx.width)
		{
			render_pixel_at_coordinates(intel->rt, x, y, inv_width, inv_height);
			x++;
		}
		y++;
	}
	
}

void	*render_all_pixels(void *intels)
{
	t_intels intel = *(t_intels *)intels;
	

    pthread_mutex_lock(&mutexQueue);
    while (ntask == 0)
	{
		pthread_cond_wait(&condQueue, &mutexQueue);
		ntask = 0;
    }

	
	pthread_mutex_unlock(&mutexQueue);
	//  future task // execute_rendering(void * task)
	return NULL;
}



/*
** render_scene - Main rendering function
*/
void target_area(t_intels *intel,t_minirt *rt)
{
	intel[0].xstart = 0;
	intel[0].xend = rt->mlx.width / 2;
	intel[0].ystart = 0;
	intel[0].yend = rt->mlx.height / 2;
	intel[0].rt = rt;

	intel[1].xstart = rt->mlx.width / 2 + 1;
	intel[1].xend = rt->mlx.width;
	intel[1].ystart = 0;
	intel[1].yend = rt->mlx.height / 2;
	intel[1].rt = rt;

	intel[2].xstart = 0;
	intel[2].xend = rt->mlx.width / 2;
	intel[2].ystart = rt->mlx.height / 2 + 1;
	intel[2].yend = rt->mlx.height;
	intel[2].rt = rt;

	intel[3].xstart = rt->mlx.width / 2 + 1;
	intel[3].xend =   rt->mlx.width;
	intel[3].ystart = rt->mlx.height / 2 + 1;
	intel[3].yend =   rt->mlx.height;
	intel[3].rt = rt;
}


void	render_scene(t_minirt *rt)
{
	pthread_t threads[NUM_THREAD];
	t_intels intels[4];

	int i;
	i = 0;
	target_area(intels,rt);
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
	while (1)
	{
		printf("tp = %d\n", ntask);
		usleep(500000);
		pthread_mutex_lock(&mutexQueue);
		pthread_mutex_unlock(&mutexQueue);
		pthread_cond_signal(&condQueue); // signal a pthread_cond_wait que sa condition a peut etre ete modifie
	}
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