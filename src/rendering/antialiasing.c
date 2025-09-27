/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   antialiasing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/27 07:41:31 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** Anti-aliasing sample offsets for 4x MSAA
** These offsets are applied to pixel coordinates for sub-pixel sampling
*/
static const double	g_offsets[4][2] = {
{-0.25, -0.25},
{0.25, -0.25},
{-0.25, 0.25},
{0.25, 0.25}
};

static inline void	set_antial(t_antial *a)
{
	a->u = 0.0;
	a->v = 0.0;
	a->ray = (t_ray){
		.origin = vec3_new(0, 0, 0),
		.direction = vec3_new(0, 0, 0)
	};
	a->final_color = (t_color){0, 0, 0};
	a->sample_color = (t_color){0, 0, 0};
}

static inline void	set_vars(int *sample, int *r, int *g, int *b)
{
	*r = 0;
	*g = 0;
	*b = 0;
	*sample = 0;
}

// >> by 2 is equivalent to / 4
t_color	render_pixel_antialiased(t_minirt *rt, int x, int y)
{
	t_antial	a;
	int			sample;
	int			total_r;
	int			total_g;
	int			total_b;

	set_antial(&a);
	set_vars(&sample, &total_r, &total_g, &total_b);
	while (sample < 4)
	{
		a.u = ((double)x + g_offsets[sample][0]) / (double)rt->mlx.width;
		a.v = ((double)y + g_offsets[sample][1]) / (double)rt->mlx.height;
		a.ray = get_camera_ray(rt, &rt->scene.camera, a.u, a.v);
		a.sample_color = raycast(a.ray, &rt->scene, MAX_DEPTH);
		total_r += a.sample_color.r;
		total_g += a.sample_color.g;
		total_b += a.sample_color.b;
		sample++;
	}
	a.final_color.r = total_r >> 2;
	a.final_color.g = total_g >> 2;
	a.final_color.b = total_b >> 2;
	return (a.final_color);
}
