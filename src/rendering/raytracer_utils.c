/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/28 19:45:12 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_pixel_world_position - Convert screen coords to world position
*/
t_vec3	calculate_pixel_world_position(t_minirt *rt, t_camera *c, double u,
		double v)
{
	double	hh;
	double	hw;
	double	aspect_ratio;
	double	fov_radians;

	fov_radians = c->fov * M_PI / 180.0;
	aspect_ratio = (double)rt->mlx.width / (double)rt->mlx.height;
	hh = tan(fov_radians * 0.5);
	hw = aspect_ratio * (hh);
	return (vec3_add(
			vec3_add(
				vec3_add(c->position, c->direction),
				vec3_mult(c->right, (u - 0.5) * 2.0 * hw)
			),
			vec3_mult(c->up, (0.5 - v) * 2.0 * hh)
		)
	);
}

/*
** calculate_background_color - Calculate background color based on ray
** currently set @ (135, 206, 255) for light sky blue
*/
t_color	calculate_background_color(t_ray ray)
{
	return ((t_color){
		(int)(135 + ray.direction.y * 50),
		(int)(206 + ray.direction.y * 49),
		255
	});
}

/*
** render_pixel_at_coordinates - Render a single pixel at given coordinates
*/
void	render_pixel_at_coordinates(t_minirt *rt, int x, int y)
{
	double	u;
	double	v;
	t_ray	ray;
	t_color	pixel_color;

	u = (double)x / rt->mlx.width;
	v = (double)y / rt->mlx.height ;
	ray = get_camera_ray(rt, &rt->scene.camera, u, v);
	pixel_color = color_clamp(raycast(ray, &rt->scene, MAX_DEPTH));
	put_pixel(&rt->mlx, x, y, pixel_color);
}
