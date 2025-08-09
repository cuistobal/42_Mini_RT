/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 09:47:08 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** ray_new - Create a new ray with given origin and direction
** The direction vector should be normalized for proper calculations
*/
t_ray	ray_new(t_vec3 origin, t_vec3 direction)
{
	t_ray	ray;

	ray.origin = origin;
	ray.direction = vec3_normalize(direction);
	return (ray);
}

/*
** ray_at - Calculate point along ray at parameter t
** Formula: P(t) = origin + t * direction
** t represents the distance along the ray from the origin
*/
t_vec3	ray_at(t_ray ray, double t)
{
	t_vec3	point;

	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	return (point);
}

/*
** ray_transform - Transform a ray by translating its origin
** Useful for moving rays in world space
*/
t_ray	ray_transform(t_ray ray, t_vec3 translation)
{
	t_ray	transformed;

	transformed.origin = vec3_add(ray.origin, translation);
	transformed.direction = ray.direction;
	return (transformed);
}

/*
** ray_reflect - Create a reflected ray from a surface
** Formula: R = D - 2(D·N)N where D is incident direction, N is normal
*/
t_ray	ray_reflect(t_ray incident, t_vec3 hit_point, t_vec3 normal)
{
	t_ray	reflected;
	double	dot_product;
	t_vec3	reflection_dir;

	dot_product = vec3_dot(incident.direction, normal);
	reflection_dir = vec3_sub(incident.direction, 
		vec3_mult(normal, 2.0 * dot_product));
	
	reflected.origin = hit_point;
	reflected.direction = vec3_normalize(reflection_dir);
	return (reflected);
}

/*
** ray_distance_to_point - Calculate distance from ray origin to a point
** along the ray direction (parameter t)
*/
double	ray_distance_to_point(t_ray ray, t_vec3 point)
{
	t_vec3	to_point;
	double	t;

	to_point = vec3_sub(point, ray.origin);
	t = vec3_dot(to_point, ray.direction);
	return (t);
}
