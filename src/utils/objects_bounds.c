/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bounds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:27:19 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 08:56:45 by chrleroy         ###   ########.fr       */
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

static inline void	triangle_based_bounds(t_object *obj, t_aabb *bounds)
{
	double epsilon = EPSILON * 10;
	
	double (minx) = fmin(obj->position.x, fmin(obj->normal.x, obj->axis.x)) \
		- epsilon;
	double (miny) = fmin(obj->position.y, fmin(obj->normal.y, obj->axis.y)) \
		- epsilon;
	double (minz) = fmin(obj->position.z, fmin(obj->normal.z, obj->axis.z)) \
		- epsilon;
	double (maxx) = fmax(obj->position.x, fmax(obj->normal.x, obj->axis.x)) \
		+ epsilon;
	double (maxy) = fmax(obj->position.y, fmax(obj->normal.y, obj->axis.y)) \
		+ epsilon;
	double (maxz) = fmax(obj->position.z, fmax(obj->normal.z, obj->axis.z)) \
		+ epsilon;
	bounds->min = vec3_new(minx, miny, minz);
	bounds->max = vec3_new(maxx, maxy, maxz);
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
	else if (obj->type == TRIANGLE)
		triangle_based_bounds(obj, &bounds);
	else
	{
		bounds.min = obj->position;
		bounds.max = obj->position;
	}
	return (bounds);
}
