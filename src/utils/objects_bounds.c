/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bounds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:27:19 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 08:33:28 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static inline void	radius_based_bounds(t_object *obj, \
		t_aabb *bounds, double *r)
{
	*r = obj->radius;
	bounds->min = vec3_sub(obj->position, vec3_new(*r, *r, *r));
	bounds->max = vec3_add(obj->position, vec3_new(*r, *r, *r));
}

static inline void	radius_and_height_based_bounds(t_object *obj, \
		t_aabb *bounds, double *r)
{
	*r = obj->radius + obj->height * 0.5;
	bounds->min = vec3_sub(obj->position, vec3_new(*r, *r, *r));
	bounds->max = vec3_add(obj->position, vec3_new(*r, *r, *r));
}

/*
** BVH (Bounding Volume Hierarchy) implementation for spatial acceleration
** This significantly reduces intersection tests for complex scenes
*/

/*
** get_object_bounds - Calculate axis-aligned bounding box for an object
** integrate SAH instead of creating a box per object. This neglects the whole
** point of the bvh.
*/
t_aabb	get_object_bounds(t_object *obj)
{
	double	r;
	t_aabb	bounds;

	if (!obj)
		return ((t_aabb){{INFINITY, INFINITY, INFINITY}, \
				{-INFINITY, -INFINITY, -INFINITY}});
	if (obj->type == SPHERE || obj->type == CUBE)
		radius_based_bounds(obj, &bounds, &r);
	else if (obj->type == CYLINDER || obj->type == CONE)
		radius_and_height_based_bounds(obj, &bounds, &r);
	else if (obj->type == PLANE)
	{
		bounds.min = vec3_new(-1000, -1000, -1000);
		bounds.max = vec3_new(1000, 1000, 1000);
	}
	else
	{
		bounds.min = obj->position;
		bounds.max = obj->position;
	}
	return (bounds);
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
