/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:56:16 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 08:57:03 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

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
	t_aabb	result;

	result.min.x = fmin(a.min.x, b.min.x);
	result.min.y = fmin(a.min.y, b.min.y);
	result.min.z = fmin(a.min.z, b.min.z);
	result.max.x = fmax(a.max.x, b.max.x);
	result.max.y = fmax(a.max.y, b.max.y);
	result.max.z = fmax(a.max.z, b.max.z);
	return (result);
}
