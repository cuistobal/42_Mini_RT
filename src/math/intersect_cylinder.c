/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	get_cylinder_root(double a, double b, double discriminant)
{
	double	sqrt_discriminant;
	double	t1;
	double	t2;
	double	inv_2a;

	inv_2a = 1.0 / (2.0 * a);
	sqrt_discriminant = sqrt(discriminant);
	t1 = (-b - sqrt_discriminant) * inv_2a;
	t2 = (-b + sqrt_discriminant) * inv_2a;
	if (t1 > EPSILON)
		return (t1);
	if (t2 > EPSILON)
		return (t2);
	return (-1.0);
}

static double	solve_quadratic_cylinder(double a, double b, double c)
{
	double	discriminant;
	double	t1;
	double	inv_2a;

	if (fabs(a) < EPSILON)
		return (-1.0);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	if (discriminant < EPSILON)
	{
		inv_2a = 1.0 / (2.0 * a);
		t1 = -b * inv_2a;
		if (t1 > EPSILON)
			return (t1);
		return (-1.0);
	}
	return (get_cylinder_root(a, b, discriminant));
}

static int	is_within_cylinder_height(t_vec3 hit_point, t_object *cylinder)
{
	t_vec3	center_to_hit;
	double	projection;

	center_to_hit = vec3_sub(hit_point, cylinder->position);
	projection = vec3_dot(center_to_hit, cylinder->axis);
	return (projection >= 0 && projection <= cylinder->height);
}

static t_cylinder_params	calculate_cylinder_params(t_ray ray,
	t_object *cylinder)
{
	t_vec3				oc;
	t_vec3				ray_cross_axis;
	t_vec3				oc_cross_axis;
	t_cylinder_params	params;

	oc = vec3_sub(ray.origin, cylinder->position);
	ray_cross_axis = vec3_cross(ray.direction, cylinder->axis);
	oc_cross_axis = vec3_cross(oc, cylinder->axis);
	params.a = vec3_dot(ray_cross_axis, ray_cross_axis);
	params.b = 2.0 * vec3_dot(ray_cross_axis, oc_cross_axis);
	params.c = vec3_dot(oc_cross_axis, oc_cross_axis)
		- (cylinder->radius * cylinder->radius);
	return (params);
}

double	intersect_cylinder(t_ray ray, t_object *cylinder)
{
	t_cylinder_params	params;
	double				t;
	t_vec3				hit_point;

	if (!cylinder || cylinder->type != CYLINDER)
		return (-1.0);
	params = calculate_cylinder_params(ray, cylinder);
	t = solve_quadratic_cylinder(params.a, params.b, params.c);
	if (t < 0)
		return (-1.0);
	hit_point = ray_at(ray, t);
	if (!is_within_cylinder_height(hit_point, cylinder))
		return (-1.0);
	return (t);
}
