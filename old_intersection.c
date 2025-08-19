/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:29:02 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/19 07:30:02 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** intersect_bvh_iter - Traverse BVH using an explicit stack (iterative)
** Push children in order of proximity (tmin)
*/
int intersect_bvh_iter(t_ray ray, t_bvh_node *root, t_hit *hit)
{
	t_hit			temp_hit;
	double			closest_t;
	int				(found), stack_ptr;
	t_bvh_node		(*stack[BVH_STACK_SIZE]), *node;
	t_aabb_query	query;

	found = 0;
	stack_ptr = 0;
	closest_t = INFINITY;
	if (!root || !hit)
		return 0;
	stack[stack_ptr++] = root;
	while (stack_ptr > 0)
	{
		node = stack[--stack_ptr];
		query = {ray.origin, ray.direction, node->bounds, -INFINITY, INFINITY};
		if (!intersect_aabb_query(&query))
			continue;
		if (node->objects && case_leaf_node(node, &temp_hit, ray) && temp_hit.t < closest_t)
		{
			*hit = temp_hit;
			closest_t = temp_hit.t;
			found = 1;
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
				if (tmin_left < tmin_right && stack_ptr < BVH_STACK_SIZE - 1)
				{
					stack[stack_ptr++] = node->right;
					stack[stack_ptr++] = node->left;
				}
				else if (stack_ptr < BVH_STACK_SIZE - 1)
				{
					stack[stack_ptr++] = node->left;
					stack[stack_ptr++] = node->right;
				}
			}
			else if (hit_left && stack_ptr < BVH_STACK_SIZE)
				stack[stack_ptr++] = node->left;
			else if (hit_right && stack_ptr < BVH_STACK_SIZE)
				stack[stack_ptr++] = node->right;
		}
	}
	return found;
}
