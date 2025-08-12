/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	get_object_intersection(t_ray ray, t_object *current)
{
	if (current->type == SPHERE)
		return (intersect_sphere(ray, current));
	else if (current->type == PLANE)
		return (intersect_plane(ray, current));
	else if (current->type == CYLINDER)
		return (intersect_cylinder(ray, current));
	else if (current->type == CONE)
		return (intersect_cone(ray, current));
	else if (current->type == CUBE)
		return (intersect_cube(ray, current));
	return (-1.0);
}

static void	update_closest_hit(double t, t_object *current,
	double *closest_t, t_object **closest_object)
{
	if (t > 0 && t < *closest_t)
	{
		*closest_t = t;
		*closest_object = current;
	}
}

static void	populate_hit_info(t_hit *hit, double closest_t,
	t_object *closest_object, t_ray ray)
{
	hit->t = closest_t;
	hit->point = ray_at(ray, closest_t);
	hit->normal = get_object_normal(hit->point, closest_object);
	hit->material = &closest_object->material;
	hit->object = closest_object;
}

int	intersect_scene(t_ray ray, t_scene *scene, t_hit *hit)
{
	if (!scene || !hit)
		return (0);
	return (intersect_bvh_iter(ray, scene->bvh_root, hit));
}

int	intersect_scene_linear(t_ray ray, t_scene *scene, t_hit *hit)
{
	t_object	*current;
	double		t;
	double		closest_t;
	t_object	*closest_object;

	if (!scene || !hit)
		return (0);
	closest_t = INFINITY;
	closest_object = NULL;
	current = scene->objects;
	while (current)
	{
		t = get_object_intersection(ray, current);
		update_closest_hit(t, current, &closest_t, &closest_object);
		current = current->next;
	}
	if (closest_object)
	{
		populate_hit_info(hit, closest_t, closest_object, ray);
		return (1);
	}
	return (0);
}
