/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_and_chess_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/13 12:20:28 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

// Allows color blending in cases when we have both texture and chess pattern
t_color	color_lerp(t_color a, t_color b, double t)
{
	return ((t_color){
		.r = (int)(a.r * (1.0 - t) + b.r * t),
		.g = (int)(a.g * (1.0 - t) + b.g * t),
		.b = (int)(a.b * (1.0 - t) + b.b * t)
	});
}

/* Safe pixel read (wrap) */
static t_color	xpm_get_pixel_safe(t_texture *tex, int x, int y)
{
	unsigned int	pix;

	if (!tex || !tex->data || tex->width <= 0 || tex->height <= 0)
		return ((t_color){
			.r = 0,
			.g = 0,
			.b = 0
		});
	x = x % tex->width;
	x += tex->width * (x < 0);
	y = y % tex->height;
	y += tex->height * (y < 0);
	pix = tex->data[y * tex->width + x];
	return ((t_color){
		.r = (pix >> 16) & 0xFF,
		.g = (pix >> 8) & 0xFF,
		.b = pix & 0xFF
	});
}

/* UV generation per object type */
void	get_uv_for_hit(t_hit *hit, double *u, double *v)
{
	t_vec3	p;

	if (hit->object->type == SPHERE)
	{
		p = vec3_normalize(vec3_sub(hit->point, hit->object->position));
		*u = 0.5 + atan2(p.z, p.x) / (2 * M_PI);
		*v = 0.5 - asin(p.y) / M_PI;
		return ;
	}
	if (hit->object->type == CYLINDER)
	{
		p = vec3_sub(hit->point, hit->object->position);
		*u = (atan2(p.z, p.x) + M_PI) / (2 * M_PI);
		*v = fmod(p.y / hit->object->height, 1.0);
		if (*v < 0)
			*v += 1.0;
		return ;
	}
	*u = fmod(hit->point.x, 1.0);
	*v = fmod(hit->point.z, 1.0);
	if (*u < 0)
		*u += 1.0;
	if (*v < 0)
		*v += 1.0;
}

/* Nearest sample with wrap */
t_color	sample_texture(t_texture *tex, double u, double v)
{
	int	x;
	int	y;

	if (!tex || !tex->data || tex->width <= 0 || tex->height <= 0)
		return ((t_color){.r = 0, .g = 0, .b = 0});
	x = (int)floor(u * tex->width);
	y = (int)floor(v * tex->height);
	return (xpm_get_pixel_safe(tex, x, y));
}

/* Returns the color resulting from texture sampling */
t_color	apply_texture(t_hit *hit)
{
	double		u;
	double		v;
	t_texture	*tex;

	tex = (t_texture *)hit->material->texture_addr;
	if (!tex || !tex->data)
		return (hit->material->color);
	get_uv_for_hit(hit, &u, &v);
	return (sample_texture(tex, u, v));
}
