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

#include "../../../includes/minirt.h"

/*
** Traverses both children node to find the matching leaf
*/
static int case_internal_node(t_bvh_node *node, t_hit *hit, t_ray ray)
{
	t_hit	left_hit;
	t_hit	right_hit;
	int		left_intersect;
	int		right_intersect;

	left_intersect = 0;
	right_intersect = 0;
	if (node->left)
		left_intersect = intersect_bvh(ray, node->left, &left_hit);
	if (node->right)
		right_intersect = intersect_bvh(ray, node->right, &right_hit);
	if (left_intersect && right_intersect)
	{
		if (left_hit.t < right_hit.t)
			*hit = left_hit;
		else
			*hit = right_hit;
		return (1);
	}
	else if (left_intersect)
		return (*hit = left_hit, 1);
	else if (right_intersect)
		return (*hit = right_hit, 1);
	return (0);
}

/*
** Base case -> fills the hit struct with the intersection data
*/
static int case_leaf_node(t_bvh_node *node, t_hit *hit, t_ray ray)
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
int	intersect_bvh(t_ray ray, t_bvh_node *node, t_hit *hit)
{
	double ray_dir[3];
	double ray_origin[3];
	double box_min[3];
	double box_max[3];

	if (!node || !hit)
		return (0);	
	ray_dir[0] = ray.direction.x;
	ray_dir[1] = ray.direction.y;
	ray_dir[2] = ray.direction.y;
	ray_origin[0] = ray.origin.x;
	ray_origin[1] = ray.origin.y;
	ray_origin[2] = ray.origin.z;
	box_min[0] = node->bounds.min.x;
	box_min[1] = node->bounds.min.y;
	box_min[2] = node->bounds.min.z;
	box_max[0] = node->bounds.max.x;
	box_max[1] = node->bounds.max.y;
	box_max[2] = node->bounds.max.z;
	if (!intersect_aabb(ray_origin, ray_dir, box_min, box_max))
		return (0);
	if (node->object)
		return (case_leaf_node(node, hit, ray));
	return (case_internal_node(node, hit, ray));
}
