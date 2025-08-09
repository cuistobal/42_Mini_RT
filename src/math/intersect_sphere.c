/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sphere.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	get_closest_root(double a, double b, double discriminant)
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

static double	solve_quadratic(double a, double b, double c)
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
	return (get_closest_root(a, b, discriminant));
}

double	intersect_sphere(t_ray ray, t_object *sphere)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;

	if (!sphere || sphere->type != SPHERE)
		return (-1.0);
	oc = vec3_sub(ray.origin, sphere->position);
	a = vec3_dot(ray.direction, ray.direction);
	b = 2.0 * vec3_dot(oc, ray.direction);
	c = vec3_dot(oc, oc) - (sphere->radius * sphere->radius);
	return (solve_quadratic(a, b, c));
}
