/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cube.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static t_slab_result	process_x_slab(t_ray ray, t_vec3 cube_min,
	t_vec3 cube_max, t_slab_result result)
{
	t_slab_intersections	intersections;

	if (!test_slab_intersection(ray.direction.x, ray.origin.x,
			cube_min.x, cube_max.x))
	{
		result.valid = 0;
		return (result);
	}
	intersections = calculate_slab_intersections(ray.direction.x, ray.origin.x,
		cube_min.x, cube_max.x);
	return (update_intersection_interval(result, intersections.t1,
			intersections.t2));
}

static t_slab_result	process_y_slab(t_ray ray, t_vec3 cube_min,
	t_vec3 cube_max, t_slab_result result)
{
	t_slab_intersections	intersections;

	if (!test_slab_intersection(ray.direction.y, ray.origin.y,
			cube_min.y, cube_max.y))
	{
		result.valid = 0;
		return (result);
	}
	intersections = calculate_slab_intersections(ray.direction.y, ray.origin.y,
		cube_min.y, cube_max.y);
	return (update_intersection_interval(result, intersections.t1,
			intersections.t2));
}

static t_slab_result	process_z_slab(t_ray ray, t_vec3 cube_min,
	t_vec3 cube_max, t_slab_result result)
{
	t_slab_intersections	intersections;

	if (!test_slab_intersection(ray.direction.z, ray.origin.z,
			cube_min.z, cube_max.z))
	{
		result.valid = 0;
		return (result);
	}
	intersections = calculate_slab_intersections(ray.direction.z, ray.origin.z,
		cube_min.z, cube_max.z);
	return (update_intersection_interval(result, intersections.t1,
			intersections.t2));
}

double	intersect_cube(t_ray ray, t_object *cube)
{
	t_vec3			cube_min;
	t_vec3			cube_max;
	t_slab_result	result;

	if (!cube || cube->type != CUBE)
		return (-1.0);
	get_cube_bounds(cube, &cube_min, &cube_max);
	result.t_min = -INFINITY;
	result.t_max = INFINITY;
	result.valid = 1;
	result = process_x_slab(ray, cube_min, cube_max, result);
	if (!result.valid)
		return (-1.0);
	result = process_y_slab(ray, cube_min, cube_max, result);
	if (!result.valid)
		return (-1.0);
	result = process_z_slab(ray, cube_min, cube_max, result);
	if (!result.valid)
		return (-1.0);
	if (result.t_min > EPSILON)
		return (result.t_min);
	if (result.t_max > EPSILON)
		return (result.t_max);
	return (-1.0);
}
