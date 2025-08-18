/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersections_internal_node.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:04:51 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 09:55:58 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

//
static double	append_intersected_node(t_bvh_node *node, \
		t_bvh_node **first, t_bvh_node **second, t_aabb_query query)
{
	*first = node->left;
	*second = node->right;
	return (query.tmin);
}

//
static int	build_queries(t_bvh_node *node, t_aabb_query *query, t_ray ray)
{
	query->origin = ray.origin;
	query->dir = ray.direction;
	query->box = node->left->bounds;
	return (node->left && intersect_aabb_query(query));
}

/*
** Traverses both children node to find the matching leaf
*/
int	case_internal_node(t_bvh_node *node, t_hit *hit, t_ray ray)
{
	t_hit			hit_first;
	t_bvh_node		*second;
	t_aabb_query	left_query;

	double (tmin_second);
	int (hit_left), (hit_right);
	t_bvh_node (*first);
	t_hit (hit_second);
	t_aabb_query (right_query);
	hit_left = build_queries(node, &left_query, ray);
	hit_right = build_queries(node, &right_query, ray);
	if (hit_left && (!hit_right || left_query.tmin < right_query.tmin))
		tmin_second = append_intersected_node(node, &first, &second, right_query);
	else if (hit_right)
		tmin_second = append_intersected_node(node, &second, &first, left_query);
	else
		return (0);
	if (first && intersect_bvh_iter(ray, first, &hit_first))
	{
		if (second && tmin_second < hit_first.t && intersect_bvh_iter(ray, \
					second, &hit_second) && hit_second.t < hit_first.t)
			return (*hit = hit_second, 1);
		return (*hit = hit_first, 1);
	}
	return (second && intersect_bvh_iter(ray, second, hit));
}
