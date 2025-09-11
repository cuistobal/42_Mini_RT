/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_plane.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

double	intersect_plane(t_ray ray, t_object *plane)
{
	double	denom;
	double	t;
	t_vec3	p0_to_origin;

	if (!plane || plane->type != PLANE)
		return (-1.0);
	denom = vec3_dot(ray.direction, plane->normal);
	if (fabs(denom) < EPSILON)
		return (-1.0);
	p0_to_origin = vec3_sub(plane->position, ray.origin);
	t = vec3_dot(p0_to_origin, plane->normal) / denom;
	if (t > EPSILON)
		return (t);
	return (-1.0);
}
