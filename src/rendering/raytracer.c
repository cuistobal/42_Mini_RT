/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/26 07:45:19 by chrleroy         ###   ########.fr       */
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
	pthread_mutex_init(&(rt->args.mutex_queue), NULL);
	pthread_cond_init(&(rt->args.cond_queue), NULL);
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
