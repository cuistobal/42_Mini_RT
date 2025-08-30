/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:29:02 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 09:43:08 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/* 0 => l, 1 => r */
static inline void	handle_single_hit(int hit, t_bvh_node **stack, \
		int *stack_ptr, t_bvh_node *n)
{
	if (*stack_ptr >= BVH_STACK_SIZE)
		return ;
	if (hit & 2 && (n->left))
		stack[(*stack_ptr)++] = n->left;
	else if (hit & 1 && n->right)
		stack[(*stack_ptr)++] = n->right;
}

static inline void	handle_multiple_hits(int condition, t_bvh_node **stack, \
		int *stack_ptr, t_bvh_node *n)
{
	if (*stack_ptr >= BVH_STACK_SIZE - 1)
		return ;
	if (condition)
	{
		stack[(*stack_ptr)++] = n->right;
		stack[(*stack_ptr)++] = n->left;
	}
	else
	{
		stack[(*stack_ptr)++] = n->left;
		stack[(*stack_ptr)++] = n->right;
	}
}

//
static void	handle_internal_n(t_bvh_node *n, t_ray ray, \
		t_bvh_iter_vars *v)
{
	t_internal_node_vars	iv;

	iv.hl = 0;
	iv.hr = 0;
	iv.tl = INFINITY;
	iv.tr = INFINITY;
	if (n->left)
	{
		setup_aabb_query(&iv.l_query, ray, n->left->bounds);
		iv.hl = intersect_aabb_query(&iv.l_query);
		if (iv.hl)
			iv.tl = iv.l_query.tmin;
	}
	if (n->right)
	{
		setup_aabb_query(&iv.r_query, ray, n->right->bounds);
		iv.hr = intersect_aabb_query(&iv.r_query);
		if (iv.hr)
			iv.tr = iv.r_query.tmin;
	}
	if (iv.hl && iv.hr)
		handle_multiple_hits(iv.tl < iv.tr, v->stack, &v->stack_ptr, n);
	else
		handle_single_hit(iv.hl << 1 | iv.hr, v->stack, &v->stack_ptr, n);
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
	t_bvh_iter_vars	vars;

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
			if (case_leaf_node(vars.node, &vars.temp_hit, ray) \
					&& vars.temp_hit.t < vars.closest_t)
				update_closest_hit(hit, vars.temp_hit, &vars.closest_t, \
						&vars.found);
		}
		handle_internal_n(vars.node, ray, &vars);
	}
	return (vars.found);
}
