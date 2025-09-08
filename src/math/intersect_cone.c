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
	return (get_cone_discriminant_root(a, b, discriminant));
}

static int	is_within_cone_height(t_vec3 hit_point, t_object *cone)
{
	t_vec3	center_to_hit;
	double	projection;

	center_to_hit = vec3_sub(hit_point, cone->position);
	projection = vec3_dot(center_to_hit, cone->axis);
	return (projection >= 0 && projection <= cone->height);
}

static t_cone_params	calculate_cone_coefficients(t_ray ray, t_object *cone)
{
	t_vec3			oc;
	double			tan_angle_sq;
	t_vec3			projected_ray_dir;
	t_vec3			projected_oc;
	t_cone_params	params;

	tan_angle_sq = tan(cone->angle * M_PI / 180.0);
	tan_angle_sq = tan_angle_sq * tan_angle_sq;
	oc = vec3_sub(ray.origin, cone->position);
	get_cone_projections(ray, cone, &projected_ray_dir, &projected_oc);
	params.a = vec3_dot(projected_ray_dir, projected_ray_dir)
		- tan_angle_sq * vec3_dot(ray.direction, cone->axis)
		* vec3_dot(ray.direction, cone->axis);
	params.b = 2.0 * (vec3_dot(projected_ray_dir, projected_oc) - tan_angle_sq \
			* vec3_dot(ray.direction, cone->axis) * vec3_dot(oc, cone->axis));
	params.c = vec3_dot(projected_oc, projected_oc)
		- tan_angle_sq * vec3_dot(oc, cone->axis)
		* vec3_dot(oc, cone->axis);
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
