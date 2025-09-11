/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersections_leaf_node.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:39:06 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 09:53:54 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	get_t(t_object *obj, t_ray ray)
{
	if (obj->type == SPHERE)
		return (intersect_sphere(ray, obj));
	else if (obj->type == PLANE)
		return (intersect_plane(ray, obj));
	else if (obj->type == CYLINDER)
		return (intersect_cylinder(ray, obj));
	else if (obj->type == CONE)
		return (intersect_cone(ray, obj));
	else if (obj->type == CUBE)
		return (intersect_cube(ray, obj));
	else if (obj->type == TRIANGLE)
		return (intersect_triangle(ray, obj));
	return (-1.0);
}

static t_hit	append_intersection_results(t_object *obj, t_ray ray, double t)
{
	t_vec3	point;

	point = ray_at(ray, t);
	return ((t_hit){
		t,
		point,
		get_object_normal(point, obj),
		&obj->material,
		obj});
}

/*
** Base case -> fills the hit struct with the intersection data
*/
int	case_leaf_node(t_bvh_node *node, t_hit *hit, t_ray ray)
{
	int		i;
	int		found;
	double	t;
	double	closest_t;	
	t_hit	temp_hit;

	i = 0;
	found = 0;
	closest_t = INFINITY;
	while (i < node->object_count)
	{
		t = get_t(node->objects[i], ray);
		if (t > 0 && t < closest_t)
		{
			closest_t = t;
			temp_hit = append_intersection_results(node->objects[i], ray, t);
			found = 1;
		}
		i++;
	}
	if (found)
		*hit = temp_hit;
	return (found);
}
