/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static t_vec3	get_cube_normal(t_vec3 hit_point, t_object *cube)
{
	t_vec3	abs_diff;
	t_vec3	center_to_hit;

	center_to_hit = vec3_sub(hit_point, cube->position);
	abs_diff.x = fabs(center_to_hit.x);
	abs_diff.y = fabs(center_to_hit.y);
	abs_diff.z = fabs(center_to_hit.z);
	if (abs_diff.x >= abs_diff.y && abs_diff.x >= abs_diff.z)
	{
		if (center_to_hit.x > 0)
			return (vec3_new(1.0, 0.0, 0.0));
		return (vec3_new(-1.0, 0.0, 0.0));
	}
	else if (abs_diff.y >= abs_diff.x && abs_diff.y >= abs_diff.z)
	{
		if (center_to_hit.y > 0)
			return (vec3_new(0.0, 1.0, 0.0));
		return (vec3_new(0.0, -1.0, 0.0));
	}
	if (center_to_hit.z > 0)
		return (vec3_new(0.0, 0.0, 1.0));
	return (vec3_new(0.0, 0.0, -1.0));
}

static inline t_vec3	get_triangle_normal(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
	return (vec3_normalize(vec3_cross(vec3_sub(v1, v0), vec3_sub(v2, v0))));
}

t_vec3	get_object_normal(t_vec3 hit_point, t_object *object)
{
	if (object->type == SPHERE)
		return (get_sphere_normal(hit_point, object));
	else if (object->type == PLANE)
		return (get_plane_normal(hit_point, object));
	else if (object->type == CYLINDER)
		return (get_cylinder_normal(hit_point, object));
	else if (object->type == CONE)
		return (get_cone_normal(hit_point, object));
	else if (object->type == CUBE)
		return (get_cube_normal(hit_point, object));
	else if (object->type == TRIANGLE)
		return (get_triangle_normal(object->position, object->normal, object->axis));
	return (vec3_new(0, 1, 0));
}
