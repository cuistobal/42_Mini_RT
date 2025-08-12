/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/12 11:13:01 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

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
	{
		r = obj->radius;
		bounds.min = vec3_sub(obj->position, vec3_new(r, r, r));
		bounds.max = vec3_add(obj->position, vec3_new(r, r, r));
	}
	else if (obj->type == CYLINDER || obj->type == CONE)
	{
		r = obj->radius + obj->height * 0.5;
		bounds.min = vec3_sub(obj->position, vec3_new(r, r, r));
		bounds.max = vec3_add(obj->position, vec3_new(r, r, r));
	}
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

/*
** count_objects - Count objects in linked list
*/
int	count_objects(t_object *objects)
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

/*
** build_bvh_recursive - Recursively build BVH tree
*/
t_bvh_node	*build_bvh_recursive(t_object **objects, int count)
{
	t_bvh_node	*node;
	t_aabb		bounds;
	int			(i), (mid), (axis), (split);
	if (count <= 0)
		return (NULL);
	node = safe_malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	bounds = get_object_bounds(objects[0]);
	i = 1;
	while (i < count)
	{
		bounds = aabb_union(bounds, get_object_bounds(objects[i]));
		i++;
	}
	node->bounds = bounds;
	   // Leaf node case (seuil fixe)
	   if (count <= MAX_OBJECTS_PER_LEAF)
	   {
			   node->objects = safe_malloc(sizeof(t_object *) * count);
			   for (i = 0; i < count; i++)
					   node->objects[i] = objects[i];
			   node->object_count = count;
			   node->left = NULL;
			   node->right = NULL;
			   return (node);
	   }
	   node->objects = NULL;
	   node->object_count = 0;
	if (count > 2)
	{
		find_sah_split(objects, count, &axis, &split);
		sort_objects_axis(objects, count, axis);
		node->left = build_bvh_recursive(objects, split);
		node->right = build_bvh_recursive(objects + split, count - split);
	}
	else
	{
		mid = count / 2;
		node->left = build_bvh_recursive(objects, mid);
		node->right = build_bvh_recursive(objects + mid, count - mid);
	}
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