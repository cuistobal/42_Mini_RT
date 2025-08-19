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

static void	setup_aabb_query(t_aabb_query *query, t_ray ray, t_aabb bounds)
{
    query->origin = ray.origin;
    query->dir = ray.direction;
    query->box = bounds;
	query->tmin = -INFINITY;
	query->tmax = INFINITY;
}

static inline void	handle_single_hit(int hit, t_bvh_node **stack, int *stack_ptr, t_bvh_node *node)
{
	if (*stack_ptr >= BVH_STACK_SIZE - 1)
		return ;
    if (hit & 4)
        stack[(*stack_ptr)++] = node->left;
    else if (hit & 1)
        stack[(*stack_ptr)++] = node->right;
}

static inline void	handle_multiple_hits(int condition, t_bvh_node **stack, int *stack_ptr, t_bvh_node *node)
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
static void	handle_internal_node(
    t_bvh_node *node, t_ray ray,
    t_bvh_node **stack, int *stack_ptr)
{
    int				(hit_left), hit_right;
    double			(tmin_left), tmin_right;
    t_aabb_query	(left_query), right_query;

    hit_left = 0;
    hit_right = 0;
    tmin_left = INFINITY;
    tmin_right = INFINITY;
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
		handle_multiple_hits(tmin_left < tmin_right, stack, stack_ptr, node);
	else
		handle_single_hit(hit_left << 4 | hit_right , stack, stack_ptr, node);
}

//
static inline void	update_closest_hit(t_hit *hit, t_hit temp_hit, double *closest_t, int *found)
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
    t_hit			temp_hit;
    double			closest_t;
    int				(found), stack_ptr;
    t_bvh_node		(*node), *stack[BVH_STACK_SIZE];
    t_aabb_query	query;

    found = 0;
    stack_ptr = 0;
    closest_t = INFINITY;
    if (!root || !hit)
        return (0);
    stack[stack_ptr++] = root;
    while (stack_ptr > 0)
    {
		node = stack[--stack_ptr];
		setup_aabb_query(&query, ray, node->bounds);
        if (!intersect_aabb_query(&query))
            continue;
        if (node->objects && case_leaf_node(node, &temp_hit, ray)
            && temp_hit.t < closest_t)
            update_closest_hit(hit, temp_hit, &closest_t, &found);
        else
            handle_internal_node(node, ray, stack, &stack_ptr);
    }
    return (found);
}