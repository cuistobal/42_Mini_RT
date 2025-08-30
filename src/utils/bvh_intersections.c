/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:29:02 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 09:25:21 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/* 0 => left, 1 => right */
static inline void	handle_single_hit(int hit, t_bvh_node **stack, \
		int *stack_ptr, t_bvh_node *node)
{
	if (*stack_ptr >= BVH_STACK_SIZE)
		return ;
	if (hit & 2 && (node->left))
		stack[(*stack_ptr)++] = node->left;
	else if (hit & 1 && node->right)
		stack[(*stack_ptr)++] = node->right;
}

static inline void	handle_multiple_hits(int condition, t_bvh_node **stack, \
		int *stack_ptr, t_bvh_node *node)
{
	if (*stack_ptr >= BVH_STACK_SIZE - 1)
		return ;
	if (condition)
	{
		stack[(*stack_ptr)++] = node->right;
		stack[(*stack_ptr)++] = node->left;
	}
	else
	{
		stack[(*stack_ptr)++] = node->left;
		stack[(*stack_ptr)++] = node->right;
	}
}

//
static void	handle_internal_node(t_bvh_node *node, t_ray ray, \
	 t_bvh_iter_vars *vars)
{
	 int hit_left = 0;
	 int hit_right = 0;
	 double tmin_left = INFINITY;
	 double tmin_right = INFINITY;
	 t_aabb_query left_query;
	 t_aabb_query right_query;
	 if (node->left)
	 {
	 setup_aabb_query(&left_query, ray, node->left->bounds);
	 hit_left = intersect_aabb_query(&left_query);
	 if (hit_left)
	 tmin_left = left_query.tmin;
	 }
	 if (node->right)
	 {
	 setup_aabb_query(&right_query, ray, node->right->bounds);
	 hit_right = intersect_aabb_query(&right_query);
	 if (hit_right)
	 tmin_right = right_query.tmin;
	 }
	 if (hit_left && hit_right)
	 handle_multiple_hits(tmin_left < tmin_right, vars->stack, &vars->stack_ptr, node);
	 else
	 handle_single_hit(hit_left << 1 | hit_right, vars->stack, &vars->stack_ptr, node);
}

//
static inline void	update_closest_hit(t_hit *hit, t_hit temp_hit, \
		 double *closest_t, int *found)
{
		 *hit = temp_hit;
		 *closest_t = temp_hit.t;
		 *found = 1;
}

/*
** Traverse BVH using an explicit stack (iterative)
** Push children in order of proximity (tmin)
*/
int	intersect_bvh_iter(t_ray ray, t_bvh_node *root, t_hit *hit)
{
	t_bvh_iter_vars vars;

	vars.found = 0;
	vars.stack_ptr = 0;
	vars.closest_t = INFINITY;
	if (!root)
		return (0);
	vars.stack[vars.stack_ptr++] = root;
	while (vars.stack_ptr > 0)
	{
		vars.node = vars.stack[--vars.stack_ptr];
		if (vars.node->objects && vars.node->object_count > 0)
		{
			if (case_leaf_node(vars.node, &vars.temp_hit, ray) && vars.temp_hit.t < vars.closest_t)
				update_closest_hit(hit, vars.temp_hit, &vars.closest_t, &vars.found);
		}
			handle_internal_node(vars.node, ray, &vars);
	}
	return (vars.found);
}
