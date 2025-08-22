/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/18 08:53:02 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
**
*/
static void	leaf_node_case(t_object **objects, t_bvh_node *node, int count)
{
	int	i;

	i = 0;
	node->objects = safe_malloc(sizeof(t_object *) * count);
	while (i < count)
	{
		node->objects[i] = objects[i];
		i++;
	}
	node->object_count = count;
	node->left = NULL;
	node->right = NULL;
}

/*
**
*/
static void	internal_node_case(t_object **objects, t_bvh_node *node, int count)
{
	//int	axis;
	int	split;

	//axis = 0;
	split = count >> 1; 
	//find_sah_split(objects, count, &axis);
	//sort_objects_axis(objects, count, axis);
	node->left = build_bvh_recursive(objects, split);
	node->right = build_bvh_recursive(objects + split, count - split);
}
/* 
static inline void	terminal_node_case(t_object **objects, t_bvh_node *node, int count)
{
	int	mid;

	mid = count / 2;
	node->left = build_bvh_recursive(objects, mid);
	node->right = build_bvh_recursive(objects + mid, count - mid);
}
 */
/*
** build_bvh_recursive - Recursively build BVH tree
** i was previously set to 1, count was previously set to 2
*/
t_bvh_node	*build_bvh_recursive(t_object **objects, int count)
{
	int			i;
	t_bvh_node	*node;
	t_aabb		bounds;

	if (count <= 0)
		return (NULL);
	node = safe_malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	i = 0;
	/* Initialize bounds with the first object to avoid out-of-bounds access when count == 1 */
	bounds = get_object_bounds(objects[0]);
	i = 1;
	while (i < count)
	{
		bounds = aabb_union(bounds, get_object_bounds(objects[i]));
		i++;
	}
	node->bounds = bounds;
	if (count <= MAX_OBJECTS_PER_LEAF)
		return (leaf_node_case(objects, node, count), node);
	node->objects = NULL;
	node->object_count = 0;
	return (internal_node_case(objects, node, count), node);
}
