/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/18 08:43:37 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** count_objects - Count objects in linked list
*/
static inline int	count_objects(t_object *objects)
{
	int	count;

	count = 0;
	while (objects)
	{
		count++;
		objects = objects->next;
	}
	return (count);
}

static void	leaf_node_case(t_object **objects, t_bvh_node *node, int count)
{
	int i;

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

t_bvh_node	*build_bvh_recursive(t_object **objects, int count);

static void	internal_node_case(t_object **objects, t_bvh_node *node, int count)
{
	int axis;
	int split;

	axis = 0;
	split = 0;
	find_sah_split(objects, count, &axis, &split);
	sort_objects_axis(objects, count, axis);
	node->left = build_bvh_recursive(objects, split);
	node->right = build_bvh_recursive(objects + split, count - split);
}

static void	terminal_node_case(t_object **objects, t_bvh_node *node, int count)
{
	int mid;

	mid = count / 2;
	node->left = build_bvh_recursive(objects, mid);
	node->right = build_bvh_recursive(objects + mid, count - mid);
}

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
	bounds = get_object_bounds(objects[0]);
	while (i < count)
		bounds = aabb_union(bounds, get_object_bounds(objects[i++]));
	node->bounds = bounds;
	if (count <= MAX_OBJECTS_PER_LEAF)
		return (leaf_node_case(objects, node, count), node);
	node->objects = NULL;
	node->object_count = 0;
	if (count > MAX_OBJECTS_PER_LEAF)
		internal_node_case(objects, node, count);
	else
		terminal_node_case(objects, node, count);
	return (node);
}

/*
** build_bvh - Build BVH from scene objects
*/
t_bvh_node	*build_bvh(t_scene *scene)
{
	int			i;
	int			count;
	t_bvh_node	*root;
	t_object	*current;
	t_object	**object_array;

	if (!scene || !scene->objects)
		return (NULL);	
	// Count objects -> that shit sucks, we can keep trqack of the obj count
	// while parsing
	count = count_objects(scene->objects);
	if (count == 0)
		return (NULL);	
	// Create array of object pointers for easier manipulation
	object_array = safe_malloc(sizeof(t_object *) * count);
	if (!object_array)
		return (NULL);	
	i = 0;
	current = scene->objects;
	while (current && i < count)
	{
		object_array[i] = current;
		current = current->next;
		i++;
	}
	root = build_bvh_recursive(object_array, count);	
	return (safe_free((void **)&object_array), root);
}

/*
** cleanup_bvh - Free BVH tree memory
*/
void	cleanup_bvh(t_bvh_node *node)
{
	if (!node)
		return ;
	cleanup_bvh(node->left);
	cleanup_bvh(node->right);
	if (node->objects)
		safe_free((void **)&node->objects);
	safe_free((void **)&node);
}


static double get_axis_value(t_object *obj, int axis)
{
	if (axis == 0)
		return (obj->position.x);
	else if (axis == 1)
		return (obj->position.y);
	return (obj->position.z);
}

static void swap_objects(t_object **a, t_object **b)
{
	t_object *tmp = *a;
	*a = *b;
	*b = tmp;
}

static int partition(t_object **arr, int low, int high, int axis)
{
	double pivot = get_axis_value(arr[high], axis);
	int i = low - 1;
	int j = low;
	while (j < high)
	{
		if (get_axis_value(arr[j], axis) < pivot)
		{
			i++;
			swap_objects(&arr[i], &arr[j]);
		}
		j++;
	}
	swap_objects(&arr[i + 1], &arr[high]);
	return (i + 1);
}

static void quicksort_objects(t_object **arr, int low, int high, int axis)
{
	int pivot;
	if (low < high)
	{
		pivot = partition(arr, low, high, axis);
		quicksort_objects(arr, low, pivot - 1, axis);
		quicksort_objects(arr, pivot + 1, high, axis);
	}
}

/*
** sort_objects_axis - Naive bubble sort for BVH splitting (axis: 0=x, 1=y, 2=z)
*/
void sort_objects_axis(t_object **objects, int count, int axis)
{
	if (count > 1)
		quicksort_objects(objects, 0, count - 1, axis);
}
