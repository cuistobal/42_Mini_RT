/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 07:36:13 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/27 07:37:21 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** is_in_shadow - Check if a point is in shadow from a specific light
** point: 3D point to test for shadows
** light: Light source to test occlusion from
** scene: Scene containing objects that can cast shadows
** Returns: 1 if point is in shadow, 0 if illuminated
*/
int	is_in_shadow_with_dir(t_vec3 point, t_vec3 dir_to_light,
		double light_distance, t_scene *scene)
{
	t_ray	shadow_ray;
	t_vec3	shadow_origin;
	t_hit	shadow_hit;

	shadow_origin = vec3_add(point, vec3_mult(dir_to_light, EPSILON));
	shadow_ray = ray_new(shadow_origin, dir_to_light);
	return (intersect_bvh_iter(shadow_ray, scene->bvh_root, &shadow_hit)
		&& (shadow_hit.t < light_distance - EPSILON));
}
