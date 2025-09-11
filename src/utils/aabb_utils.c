/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:56:16 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/19 08:38:08 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** create_empty_aabb - Create an empty AABB with infinite bounds
** Used as a starting point for AABB computations
*/
t_aabb	create_empty_aabb(void)
{
	t_aabb	aabb;

	aabb.min.x = INFINITY;
	aabb.min.y = INFINITY;
	aabb.min.z = INFINITY;
	aabb.max.x = -INFINITY;
	aabb.max.y = -INFINITY;
	aabb.max.z = -INFINITY;
	return (aabb);
}

/*
** Computes the surface area of an axis-aligned bounding box (AABB)
*/
double	aabb_surface(t_aabb a)
{
	double	dx;
	double	dy;
	double	dz;

	dx = fabs(a.max.x - a.min.x);
	dy = fabs(a.max.y - a.min.y);
	dz = fabs(a.max.z - a.min.z);
	return (2.0 * (dx * dy + dx * dz + dy * dz));
}

/*
** aabb_union - Combine two AABBs into one that contains both
*/
t_aabb	aabb_union(t_aabb a, t_aabb b)
{
	return ((t_aabb){
		.min.x = fmin(a.min.x, b.min.x),
		.min.y = fmin(a.min.y, b.min.y),
		.min.z = fmin(a.min.z, b.min.z),
		.max.x = fmax(a.max.x, b.max.x),
		.max.y = fmax(a.max.y, b.max.y),
		.max.z = fmax(a.max.z, b.max.z)
	});
}

/*
** Basic setter used in the bvh intersection function
**/
void	setup_aabb_query(t_aabb_query *query, t_ray ray, t_aabb bounds)
{
	query->origin = ray.origin;
	query->dir = ray.direction;
	query->box = bounds;
	query->tmin = -INFINITY;
	query->tmax = INFINITY;
}
