/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_maps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/13 11:36:25 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/* Build tangent frame (tangent, bitangent) from a normal. Keeps local state
 * minimal so callers can handle height sampling separately.
 */
static void	compute_tangent_frame(t_vec3 normal, t_vec3 *tangent, \
		t_vec3 *bitangent)
{
	t_vec3	up;

	if (fabs(normal.y) < 0.99)
		up = vec3_new(0, 1, 0);
	else
		up = vec3_new(1, 0, 0);
	*tangent = vec3_normalize(vec3_cross(up, normal));
	*bitangent = vec3_cross(normal, *tangent);
}

/* Sample heightmap and return deltas hx,hy as a vec3 (x=lx, y=ly). Returns
 * (hx, hy, 0). Keeps parameter count <=4 and local declarations small.
 */
static t_vec3	compute_height_deltas(t_texture *bump, double u, double v)
{
	double	h;
	t_color	c;
	t_color	cx;
	t_color	cy;

	c = sample_texture(bump, u, v);
	cx = sample_texture(bump, u + DBL_EPSILON, v);
	cy = sample_texture(bump, u, v + DBL_EPSILON);
	h = (c.r + c.g + c.b) / (3.0 * 255.0);
	return ((t_vec3){
		.x = ((cx.r + cx.g + cx.b) / (3.0 * 255.0)) - h,
		.y = ((cy.r + cy.g + cy.b) / (3.0 * 255.0)) - h,
		.z = 0.0
	});
}

/* Looks for unregularities in the bump map -> this is the magic trick
 * to produce the relief
 */
static t_vec3	compute_perturbation(t_vec3 normal, double u, double v, \
		t_texture *bump)
{
	t_vec3	pert;
	t_vec3	deltas;
	t_vec3	tangent;
	t_vec3	bitangent;

	compute_tangent_frame(normal, &tangent, &bitangent);
	deltas = compute_height_deltas(bump, u, v);
	pert = vec3_add(vec3_mult(tangent, deltas.x * 10.0), \
			vec3_mult(bitangent, deltas.y * 10.0));
	return (pert);
}

/* Height map approach bumping */
t_vec3	apply_bump_map_if_present(t_hit *hit, t_vec3 normal, double u, double v)
{
	t_vec3	pert;

	if (!hit->material->texture_addr)
		return (normal);
	pert = compute_perturbation(normal, u, v, \
			hit->material->texture_addr);
	return (vec3_normalize(vec3_add(normal, pert)));
}
