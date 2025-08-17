/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   antialiasing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/07/30 07:33:02 by cuistobal        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** Anti-aliasing sample offsets for 4x MSAA
** These offsets are applied to pixel coordinates for sub-pixel sampling
*/
static const double g_sample_offsets[4][2] = {
	{-0.25, -0.25},
	{0.25, -0.25},
	{-0.25, 0.25},
	{0.25, 0.25}
};

/*
** render_pixel_antialiased - Render a single pixel with anti-aliasing
** rt: Main program structure
** x, y: Pixel coordinates
** Returns: Anti-aliased color for the pixel
*/
t_color	render_pixel_antialiased(t_minirt *rt, int x, int y)
{
	t_color	final_color;
	t_color	sample_color;
	t_ray	ray;
	double	u;
	double	v;
	int		sample;
	int		total_r;
	int		total_g;
	int		total_b;

	if (!rt)
		return (color_new(0, 0, 0));
	
	total_r = 0;
	total_g = 0;
	total_b = 0;
	
	// Take multiple samples per pixel
	for (sample = 0; sample < 4; sample++)
	{
		// Calculate sub-pixel coordinates with sample offset
		u = ((double)x + g_sample_offsets[sample][0]) / (double)rt->mlx.width;
		v = ((double)y + g_sample_offsets[sample][1]) / (double)rt->mlx.height;
		
		// Generate camera ray for this sub-pixel
		ray = get_camera_ray(rt, &rt->scene.camera, u, v);
		
		// Cast ray and get color
		sample_color = raycast(ray, &rt->scene, MAX_DEPTH);
		
		// Accumulate color values
		total_r += sample_color.r;
		total_g += sample_color.g;
		total_b += sample_color.b;
	}
	
	// Average the samples
	final_color.r = total_r >> 2;
	final_color.g = total_g >> 2;
	final_color.b = total_b >> 2;

	return (final_color);
}