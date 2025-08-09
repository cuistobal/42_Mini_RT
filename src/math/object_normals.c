/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_normals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 10:19:17 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vec3	get_sphere_normal(t_vec3 hit_point, t_object *sphere)
{
	t_vec3	normal;

	normal = vec3_sub(hit_point, sphere->position);
	return (vec3_normalize(normal));
}

t_vec3	get_plane_normal(t_vec3 hit_point, t_object *plane)
{
	(void)hit_point;
	return (vec3_normalize(plane->normal));
}

t_vec3	get_cylinder_normal(t_vec3 hit_point, t_object *cylinder)
{
	t_vec3	center_to_hit;
	t_vec3	axis_projection;
	t_vec3	normal;
	double	projection_length;

	center_to_hit = vec3_sub(hit_point, cylinder->position);
	projection_length = vec3_dot(center_to_hit, cylinder->axis);
	axis_projection = vec3_mult(cylinder->axis, projection_length);
	normal = vec3_sub(center_to_hit, axis_projection);
	return (vec3_normalize(normal));
}

t_vec3	get_cone_normal(t_vec3 hit_point, t_object *cone)
{
	t_vec3	center_to_hit;
	t_vec3	axis_projection;
	t_vec3	radial_vector;
	t_vec3	normal;
	double	projection_length;

	center_to_hit = vec3_sub(hit_point, cone->position);
	projection_length = vec3_dot(center_to_hit, cone->axis);
	axis_projection = vec3_mult(cone->axis, projection_length);
	radial_vector = vec3_sub(center_to_hit, axis_projection);
	normal = vec3_sub(radial_vector, vec3_mult(cone->axis, \
				vec3_length(radial_vector) * tan(cone->angle * M_PI / 180.0)));
	return (vec3_normalize(normal));
}
