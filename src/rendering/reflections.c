/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/13 11:43:50 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_reflection - Calculate reflection color contribution
** ray: Incident ray that hit the surface
** hit: Hit information containing point, normal, and material
** scene: Scene containing objects and lights
** depth: Current recursion depth for ray tracing
** Returns: Reflection color contribution
*/
t_color	calculate_reflection(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
	t_ray	reflected_ray;
	t_color	reflection_color;
	t_color	final_color;
	double	reflection_intensity;

	if (!hit || !hit->material || !scene || depth <= 0)
		return (color_new(0, 0, 0));
	reflection_intensity = hit->material->reflection;
	if (reflection_intensity <= 0.0)
		return (color_new(0, 0, 0));
	reflected_ray = ray_reflect(ray, hit->point, hit->normal);
	reflection_color = raycast(reflected_ray, scene, depth - 1);
	final_color = color_mult(reflection_color, reflection_intensity);
	return (final_color);
}

/*
** calculate_refraction_ray - Calculate refracted ray using Snell's law
** incident: Incident ray direction (normalized)
** normal: Surface normal (normalized)
** n1: Refractive index of first medium
** n2: Refractive index of second medium
** Returns: Refracted ray direction, or zero vector if total internal reflection
*/
static t_vec3	calculate_refraction_ray(t_vec3 incident, t_vec3 normal, \
	double n1, double n2)
{
	double	eta;
	double	cos_i;
	double	cos_t;
	double	sin_t_sq;
	t_vec3	refracted;

	eta = n1 / n2;
	cos_i = -vec3_dot(incident, normal);
	sin_t_sq = eta * eta * (1.0 - cos_i * cos_i);
	if (sin_t_sq > 1.0)
		return (vec3_new(0, 0, 0));
	cos_t = sqrt(1.0 - sin_t_sq);
	refracted = vec3_add(vec3_mult(incident, eta), \
			vec3_mult(normal, eta * cos_i - cos_t));
	return (vec3_normalize(refracted));
}

/*
** calculate_fresnel - Calculate Fresnel reflectance
** cos_i: Cosine of incident angle
** n1: Refractive index of first medium
** n2: Refractive index of second medium
** Returns: Fresnel reflectance coefficient (0-1)
*/
static double	calculate_fresnel(double cos_i, double n1, double n2)
{
	double	r_s;
	double	r_p;
	double	eta;
	double	cos_t;
	double	sin_t_sq;

	eta = n1 / n2;
	sin_t_sq = eta * eta * (1.0 - cos_i * cos_i);
	if (sin_t_sq > 1.0)
		return (1.0);
	cos_t = sqrt(1.0 - sin_t_sq);
	r_s = (n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t);
	r_p = (n2 * cos_i - n1 * cos_t) / (n2 * cos_i + n1 * cos_t);
	return ((r_s * r_s + r_p * r_p) * 0.5);
}

/*
** calculate_refraction - Calculate refraction color contribution
** ray: Incident ray that hit the surface
** hit: Hit information containing point, normal, and material
** scene: Scene containing objects and lights
** depth: Current recursion depth for ray tracing
** Returns: Refraction color contribution
*/
t_color	calculate_refraction(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
	t_refraction	r;

	if (!hit || !hit->material || !scene || depth <= 0 || \
			hit->material->transparency <= 0.0)
		return (color_new(0, 0, 0));
	r.transparency = hit->material->transparency;
	r.n1 = 1.0;
	r.n2 = hit->material->refraction_index;
	r.cos_i = -vec3_dot(ray.direction, hit->normal);
	r.refracted_dir = calculate_refraction_ray(ray.direction, hit->normal, \
			r.n1, r.n2);
	if (vec3_length(r.refracted_dir) < EPSILON)
		return (color_new(0, 0, 0));
	r.hit_offset = vec3_add(hit->point, vec3_mult(r.refracted_dir, EPSILON));
	r.refracted_ray = ray_new(r.hit_offset, r.refracted_dir);
	r.refraction_color = raycast(r.refracted_ray, scene, depth - 1);
	r.fresnel = calculate_fresnel(fabs(r.cos_i), r.n1, r.n2);
	return (color_mult(r.refraction_color, r.transparency * (1.0 - r.fresnel)));
}
