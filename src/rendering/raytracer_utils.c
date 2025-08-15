/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 12:53:22 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_viewport_dimensions - Calculate viewport size from FOV
*/
void	calculate_viewport_dimensions(t_minirt *rt, t_camera *camera,
	double *half_width, double *half_height)
{
	double	aspect_ratio;
	double	fov_radians;

	fov_radians = camera->fov * M_PI / 180.0;
	aspect_ratio = (double)rt->mlx.width / (double)rt->mlx.height;
	*half_height = tan(fov_radians * 0.5);
	*half_width = aspect_ratio * (*half_height);
}

/*
** calculate_pixel_world_position - Convert screen coords to world position
*/
t_vec3	calculate_pixel_world_position(t_camera *camera, double u, \
		double v, double half_width, double half_height)
{
	t_vec3	pixel_world;
	t_vec3	right_offset;
	t_vec3	up_offset;

	pixel_world = camera->position;
	pixel_world = vec3_add(pixel_world, camera->direction);
	right_offset = vec3_mult(camera->right, (u - 0.5) * 2.0 * half_width);
	up_offset = vec3_mult(camera->up, (0.5 - v) * 2.0 * half_height);
	pixel_world = vec3_add(pixel_world, right_offset);
	pixel_world = vec3_add(pixel_world, up_offset);
	return (pixel_world);
}

/*
** calculate_background_color - Calculate background color based on ray
*/
t_color	calculate_background_color(t_ray ray)
{
	t_color	background_color;

	background_color = color_new((int)(135 + ray.direction.y * 50), (int)(206 \
				+ ray.direction.y * 49), 255);
	return (color_clamp(background_color));
}

/*
** render_pixel_at_coordinates - Render a single pixel at given coordinates
*/
void	render_pixel_at_coordinates(t_minirt *rt, int x, int y,
	double inv_width, double inv_height)
{
	double	u;
	double	v;
	t_ray	ray;
	t_color	pixel_color;

	u = (double)x * inv_width; // bit shifting
	v = (double)y * inv_height; // bit shifting
	ray = get_camera_ray(rt, &rt->scene.camera, u, v);
	pixel_color = raycast(ray, &rt->scene, MAX_DEPTH);
	put_pixel(&rt->mlx, x, y, pixel_color);
}
