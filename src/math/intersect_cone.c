/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/08 12:12:15 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	get_cone_discriminant_root(double a, double b,
	double discriminant)
{
	double	t1;
	double	t2;
	double	inv_2a;
	double	sqrt_discriminant;

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

static double	solve_quadratic_cone(double a, double b, double c)
{
	double	discriminant;
	double	t1;
	double	inv_2a;

	if (fabs(a) < EPSILON)
	{
		if (fabs(b) < EPSILON)
			return (-1.0);
		t1 = -c / b;
		return (t1 > EPSILON ? t1 : -1.0);
	}
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < -EPSILON)
		return (-1.0);
	if (fabs(discriminant) < EPSILON)
	{
		inv_2a = 1.0 / (2.0 * a);
		t1 = -b * inv_2a;
		return (t1 > EPSILON ? t1 : -1.0);
	}
	return (get_cone_discriminant_root(a, b, discriminant));
}

static int	is_within_cone_height(t_vec3 hit_point, t_object *cone)
{
	t_vec3	center_to_hit;
	double	projection;

	center_to_hit = vec3_sub(hit_point, cone->position);
	projection = vec3_dot(center_to_hit, cone->axis);
	
	/* Check if hit point is within cone height bounds */
	/* Cone extends from base (projection = 0) to apex (projection = height) */
	return (projection >= -EPSILON && projection <= cone->height + EPSILON);
}

static t_cone_params	calculate_cone_coefficients(t_ray ray, t_object *cone)
{
	t_vec3			oc;
	double			cos_angle_sq;
	double			dot_rd_axis;
	double			dot_oc_axis;
	t_cone_params	params;

	cos_angle_sq = cos(cone->angle * M_PI / 180.0);
	cos_angle_sq = cos_angle_sq * cos_angle_sq;
	oc = vec3_sub(ray.origin, cone->position);
	dot_rd_axis = vec3_dot(ray.direction, cone->axis);
	dot_oc_axis = vec3_dot(oc, cone->axis);
	params.a = vec3_dot(ray.direction, ray.direction) * cos_angle_sq
		- dot_rd_axis * dot_rd_axis;
	params.b = 2.0 * (vec3_dot(ray.direction, oc) * cos_angle_sq
		- dot_rd_axis * dot_oc_axis);
	params.c = vec3_dot(oc, oc) * cos_angle_sq
		- dot_oc_axis * dot_oc_axis;
	return (params);
}

double	intersect_cone(t_ray ray, t_object *cone)
{
	t_cone_params	params;
	double			t;
	t_vec3			hit_point;

	if (!cone || cone->type != CONE)
		return (-1.0);
	params = calculate_cone_coefficients(ray, cone);
	t = solve_quadratic_cone(params.a, params.b, params.c);
	if (t < 0)
		return (-1.0);
	hit_point = ray_at(ray, t);
	if (!is_within_cone_height(hit_point, cone))
		return (-1.0);
	return (t);
}
