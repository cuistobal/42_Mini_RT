/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/28 16:01:05 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** ray_new - Create a new ray with given origin and direction
** The direction vector should be normalized for proper calculations
*/
t_ray	ray_new(t_vec3 origin, t_vec3 direction)
{
	return ((t_ray){
		.origin = origin,
		.direction = vec3_normalize(direction)
	});
}

/*
** ray_at - Calculate point along ray at parameter t
** Formula: P(t) = origin + t * direction
** t represents the distance along the ray from the origin
*/
t_vec3	ray_at(t_ray ray, double t)
{
	return (vec3_add(ray.origin, vec3_mult(ray.direction, t)));
}

/*
** ray_transform - Transform a ray by translating its origin
** Useful for moving rays in world space
*/
t_ray	ray_transform(t_ray ray, t_vec3 translation)
{
	return ((t_ray){
		.origin = vec3_add(ray.origin, translation),
		.direction = ray.direction
	});
}

/*
** ray_reflect - Create a reflected ray from a surface
** Formula: R = D - 2(D·N)N where D is incident direction, N is normal
*/
t_ray	ray_reflect(t_ray incident, t_vec3 hit_point, t_vec3 normal)
{
	double	dot_product;
	t_vec3	reflection_dir;

	dot_product = vec3_dot(incident.direction, normal);
	reflection_dir = vec3_sub(incident.direction, \
			vec3_mult(normal, 2.0 * dot_product));
	return ((t_ray){
		.origin = hit_point,
		.direction = vec3_normalize(reflection_dir)
	});
}

/*
** ray_distance_to_point - Calculate distance from ray origin to a point
** along the ray direction (parameter t)
*/
double	ray_distance_to_point(t_ray ray, t_vec3 point)
{
	return (vec3_dot(vec3_sub(point, ray.origin), ray.direction));
}
