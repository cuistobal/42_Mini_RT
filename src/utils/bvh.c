/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 16:52:38 by chrleroy         ###   ########.fr       */
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
static t_aabb	get_object_bounds(t_object *obj)
{
	double	r;
	t_aabb	bounds;

	if (!obj)
		return ((t_aabb){{INFINITY, INFINITY, INFINITY}, \
				{-INFINITY, -INFINITY, -INFINITY}});
	if (obj->type == SPHERE)
	{
		r = obj->radius;
		bounds.min = vec3_sub(obj->position, vec3_new(r, r, r));
		bounds.max = vec3_add(obj->position, vec3_new(r, r, r));
	}
	else if (obj->type == CUBE)
	{
		r = obj->radius;
		bounds.min = vec3_sub(obj->position, vec3_new(r, r, r));
		bounds.max = vec3_add(obj->position, vec3_new(r, r, r));
	}
	else if (obj->type == CYLINDER)
	{
		// Simplified AABB for cylinder (could be more precise)
		r = obj->radius + obj->height * 0.5;
		bounds.min = vec3_sub(obj->position, vec3_new(r, r, r));
		bounds.max = vec3_add(obj->position, vec3_new(r, r, r));
	}
	else if (obj->type == CONE)
	{
		// Simplified AABB for cone
		r = obj->radius + obj->height * 0.5;
		bounds.min = vec3_sub(obj->position, vec3_new(r, r, r));
		bounds.max = vec3_add(obj->position, vec3_new(r, r, r));
	}
	else if (obj->type == PLANE)
	{
		// Planes are infinite, use large bounds
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
static t_aabb	aabb_union(t_aabb a, t_aabb b)
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
** intersect_aabb - Fast ray-AABB intersection test
** Returns 1 if ray intersects the bounding box, 0 otherwise
** We need to split that shit for norminette
*/
static int	intersect_aabb(t_ray ray, t_aabb box)
{
	double	t_min;
	double	t_max;
	double	t1;
	double	t2;
	double	temp;
	int		i;
	double	ray_dir[3];
	double	ray_orig[3];
	double	box_min[3];
	double	box_max[3];

	// Convert to arrays for easier iteration
	ray_dir[0] = ray.direction.x;
	ray_dir[1] = ray.direction.y;
	ray_dir[2] = ray.direction.z;
	ray_orig[0] = ray.origin.x;
	ray_orig[1] = ray.origin.y;
	ray_orig[2] = ray.origin.z;
	box_min[0] = box.min.x;
	box_min[1] = box.min.y;
	box_min[2] = box.min.z;
	box_max[0] = box.max.x;
	box_max[1] = box.max.y;
	box_max[2] = box.max.z;	
	t_min = -INFINITY;
	t_max = INFINITY;

	// Test intersection with each pair of parallel planes
	i = 0;
	while (i < 3)
	{
		if (fabs(ray_dir[i]) < EPSILON)
		{
			// Ray is parallel to the slab
			if (ray_orig[i] < box_min[i] || ray_orig[i] > box_max[i])
				return (0);
		}
		else
		{
			t1 = (box_min[i] - ray_orig[i]) / ray_dir[i];
			t2 = (box_max[i] - ray_orig[i]) / ray_dir[i];
			if (t1 > t2)
			{
				temp = t1;
				t1 = t2;
				t2 = temp;
			}	
			if (t1 > t_min)
				t_min = t1;
			if (t2 < t_max)
				t_max = t2;
			if (t_min > t_max)
				return (0);
		}
		i++;
	}
	return (t_max > EPSILON);
}

/*
** count_objects - Count objects in linked list
*/
static int	count_objects(t_object *objects)
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
static t_bvh_node	*build_bvh_recursive(t_object **objects, int count)
{
	t_bvh_node	*node;
	t_aabb		bounds;
	int			i;
	int			mid;

	if (count <= 0)
		return (NULL);	
	node = safe_malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	// Calculate bounding box for all objects
	bounds = get_object_bounds(objects[0]);
	i = 1;
	while (i < count)
	{
		bounds = aabb_union(bounds, get_object_bounds(objects[i]));
		i++;
	}
	node->bounds = bounds;
	// Leaf node case
	if (count == 1)
	{
		node->object = objects[0];
		node->left = NULL;
		node->right = NULL;
		return (node);
	}
	// Internal node case - split objects in half (simple split)
	node->object = NULL;
	mid = count / 2;
	node->left = build_bvh_recursive(objects, mid);
	node->right = build_bvh_recursive(objects + mid, count - mid);
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
** intersect_bvh - Traverse BVH to find closest intersection
*/
int	intersect_bvh(t_ray ray, t_bvh_node *node, t_hit *hit)
{
	t_hit	left_hit;
	t_hit	right_hit;
	int		left_intersect;
	int		right_intersect;
	double	t;

	if (!node || !hit)
		return (0);
	
	// Test ray against bounding box first
	if (!intersect_aabb(ray, node->bounds))
		return (0);
	
	// Leaf node - test against object
	if (node->object)
	{
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
		
		if (t > 0)
		{
			hit->t = t;
			hit->point = ray_at(ray, t);
			hit->normal = get_object_normal(hit->point, node->object);
			hit->material = &node->object->material;
			hit->object = node->object;
			return (1);
		}
		return (0);
	}
	
	// Internal node - test both children
	left_intersect = 0;
	right_intersect = 0;
	
	if (node->left)
		left_intersect = intersect_bvh(ray, node->left, &left_hit);
	if (node->right)
		right_intersect = intersect_bvh(ray, node->right, &right_hit);
	
	// Return closest intersection
	if (left_intersect && right_intersect)
	{
		if (left_hit.t < right_hit.t)
			*hit = left_hit;
		else
			*hit = right_hit;
		return (1);
	}
	else if (left_intersect)
	{
		*hit = left_hit;
		return (1);
	}
	else if (right_intersect)
	{
		*hit = right_hit;
		return (1);
	}
	
	return (0);
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
	safe_free((void **)&node);
}
