/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal         #+#    #+#             */
/*   Updated: 2025/09/13 11:32:47 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

// Local constant colors for chess pattern
static const t_color	g_colors[2] = {
	(t_color){.r = 0, .g = 0, .b = 0},
	(t_color){.r = 255, .g = 255, .b = 255}
};

/* Chess pattern from UV (returns color, does not mutate material) */
static t_color	chess_color_from_uv(double u, double v, int scale)
{
	int	iu;
	int	iv;

	iu = (int)floor(u * (double)scale);
	iv = (int)floor(v * (double)scale);
	return (g_colors[(iu + iv) & 1]);
}

/* Prepare a temporary hit and material for lighting: computes UV,
 * applies bump, samples texture, blends with chess if present,
 * and fills out_mat/out_hit.
 */
static void	prepare_hit_for_lighting(t_hit *hit, t_hit *out_hit, \
		t_material *out_mat, double blend)
{
	double	u;
	double	v;
	t_vec3	normal;
	t_color	tex_col;
	t_color	albedo;

	get_uv_for_hit(hit, &u, &v);
	normal = apply_bump_map_if_present(hit, hit->normal, u, v);
	tex_col = apply_texture(hit);
	albedo = tex_col;
	if (hit->material->chess > 0)
		albedo = color_lerp(tex_col, \
				chess_color_from_uv(u, v, hit->material->chess), blend);
	*out_mat = *hit->material;
	out_mat->color = albedo;
	*out_hit = *hit;
	out_hit->material = out_mat;
	out_hit->normal = normal;
}

/* calculate_hit_color: do not mutate material. Create temporaries
 * for lighting.
 * UV -> bump (normal) -> texture/chess (albedo) -> lighting + effects
 */
t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
	t_hit		tmp_hit;
	t_material	tmp_mat;
	t_color		base_color;
	t_color		reflection_color;
	t_color		refraction_color;

	prepare_hit_for_lighting(hit, &tmp_hit, &tmp_mat, \
			1.0 - (0.5 * (hit->material->chess > 0)));
	base_color = calculate_lighting(tmp_hit.point, tmp_hit.normal, \
			scene, tmp_hit.material);
	reflection_color = calculate_reflection(ray, &tmp_hit, scene, depth);
	refraction_color = calculate_refraction(ray, &tmp_hit, scene, depth);
	return (color_add(color_add(base_color, reflection_color), \
				refraction_color));
}
