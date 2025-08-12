/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:29:02 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/11 09:51:12 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** Traverses both children node to find the matching leaf
*/
int case_internal_node(t_bvh_node *node, t_hit *hit, t_ray ray)
{
	t_aabb_query (left_query), right_query;
	int (hit_left), hit_right;
	t_bvh_node (*first), *second;
	double tmin_second;
	t_hit (hit_first), hit_second;

	left_query.origin = ray.origin;
	left_query.dir = ray.direction;
	left_query.box = node->left->bounds;
	hit_left = node->left && intersect_aabb_query(&left_query);

	right_query.origin = ray.origin;
	right_query.dir = ray.direction;
	right_query.box = node->right->bounds;
	hit_right = node->right && intersect_aabb_query(&right_query);

	if (hit_left && (!hit_right || left_query.tmin < right_query.tmin))
	{
		first = node->left;
		second = node->right;
		tmin_second = right_query.tmin;
	}
	else if (hit_right)
	{
		first = node->right;
		second = node->left;
		tmin_second = left_query.tmin;
	}
	else
		return (0);

	if (first && intersect_bvh_iter(ray, first, &hit_first))
	{
		if (second && tmin_second < hit_first.t && intersect_bvh_iter(ray, second, &hit_second) && hit_second.t < hit_first.t)
			*hit = hit_second;
		else
			*hit = hit_first;
		return (1);
	}
	if (second && intersect_bvh_iter(ray, second, hit))
		return (1);
	return (0);
}

/*
** Base case -> fills the hit struct with the intersection data
*/
int case_leaf_node(t_bvh_node *node, t_hit *hit, t_ray ray)
{
	double t;

	if (node->object->type == SPHERE)
		t = intersect_sphere(ray, node->object);
	else if (node->object->type == PLANE)
		t = intersect_plane(ray, node->object);
	else if (node->object->type == CYLINDER)
		t = intersect_cylinder(ray, node->object);
	else if (node->object->type == CONE)
		t = intersect_cone(ray, node->object);
	else if (node->object->type == CUBE)
		t = intersect_cube(ray, node->object);
	else
		t = -1.0;
	if (t <= 0)
		return (0);
	hit->t = t;
	hit->point = ray_at(ray, t);
	hit->normal = get_object_normal(hit->point, node->object);
	hit->material = &node->object->material;
	hit->object = node->object;
	return (1);
}

/*
** intersect_bvh - Traverse BVH to find closest intersection
*/
/* 
int	intersect_bvh(t_ray ray, t_bvh_node *node, t_hit *hit)
{
	t_aabb_query query;

	if (!node || !hit)
		return (0);
	query.origin = ray.origin;
	query.dir = ray.direction;
	query.box = node->bounds;
	if (!intersect_aabb_query(&query))
		return (0);
	if (node->object)
		return (case_leaf_node(node, hit, ray));
	return (case_internal_node(node, hit, ray));
}
 */

#define BVH_STACK_SIZE 64


/*
** intersect_bvh_iter - Traverse BVH using an explicit stack (iterative)
** Push children in order of proximity (tmin)
*/
int intersect_bvh_iter(t_ray ray, t_bvh_node *root, t_hit *hit)
{
	t_bvh_node *stack[BVH_STACK_SIZE];
	int stack_ptr = 0;
	t_hit temp_hit;
	int found = 0;
	double closest_t = INFINITY;

	if (!root || !hit)
		return 0;
	stack[stack_ptr++] = root;
	while (stack_ptr > 0)
	{
		t_bvh_node *node = stack[--stack_ptr];
		t_aabb_query query = {ray.origin, ray.direction, node->bounds, -INFINITY, INFINITY};
		if (!intersect_aabb_query(&query))
			continue;
		if (node->object)
		{
			if (case_leaf_node(node, &temp_hit, ray) && temp_hit.t < closest_t)
			{
				*hit = temp_hit;
				closest_t = temp_hit.t;
				found = 1;
			}
		}
		else
		{
			t_aabb_query left_query, right_query;
			int hit_left = 0, hit_right = 0;
			double tmin_left = INFINITY, tmin_right = INFINITY;
			if (node->left)
			{
				left_query.origin = ray.origin;
				left_query.dir = ray.direction;
				left_query.box = node->left->bounds;
				hit_left = intersect_aabb_query(&left_query);
				if (hit_left)
					tmin_left = left_query.tmin;
			}
			if (node->right)
			{
				right_query.origin = ray.origin;
				right_query.dir = ray.direction;
				right_query.box = node->right->bounds;
				hit_right = intersect_aabb_query(&right_query);
				if (hit_right)
					tmin_right = right_query.tmin;
			}
			if (hit_left && hit_right)
			{
				if (tmin_left < tmin_right)
				{
					if (stack_ptr < BVH_STACK_SIZE - 1)
					{
						stack[stack_ptr++] = node->right;
						stack[stack_ptr++] = node->left;
					}
				}
				else
				{
					if (stack_ptr < BVH_STACK_SIZE - 1)
					{
						stack[stack_ptr++] = node->left;
						stack[stack_ptr++] = node->right;
					}
				}
			}
			else if (hit_left)
			{
				if (stack_ptr < BVH_STACK_SIZE)
					stack[stack_ptr++] = node->left;
			}
			else if (hit_right)
			{
				if (stack_ptr < BVH_STACK_SIZE)
					stack[stack_ptr++] = node->right;
			}
		}
	}
	return found;
}