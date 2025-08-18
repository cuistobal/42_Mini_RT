/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
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

	// Check if we should calculate reflections
	if (!hit || !hit->material || !scene || depth <= 0)
		return (color_new(0, 0, 0));
	
	// Get reflection intensity from material
	reflection_intensity = hit->material->reflection;
	if (reflection_intensity <= 0.0)
		return (color_new(0, 0, 0));
	
	// Generate reflected ray
	reflected_ray = ray_reflect(ray, hit->point, hit->normal);
	
	// Recursively trace the reflected ray
	reflection_color = raycast(reflected_ray, scene, depth - 1);
	
	// Apply reflection intensity
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
static t_vec3	calculate_refraction_ray(t_vec3 incident, t_vec3 normal, double n1, double n2)
{
	double	cos_i;
	double	sin_t_sq;
	double	cos_t;
	double	eta;
	t_vec3	refracted;

	eta = n1 / n2;
	cos_i = -vec3_dot(incident, normal);
	sin_t_sq = eta * eta * (1.0 - cos_i * cos_i);
	
	// Check for total internal reflection
	if (sin_t_sq > 1.0)
		return (vec3_new(0, 0, 0)); // Total internal reflection
	
	cos_t = sqrt(1.0 - sin_t_sq);
	refracted = vec3_add(
		vec3_mult(incident, eta),
		vec3_mult(normal, eta * cos_i - cos_t)
	);
	
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
	double	sin_t_sq;
	double	cos_t;
	double	r_s;
	double	r_p;
	double	eta;

	eta = n1 / n2;
	sin_t_sq = eta * eta * (1.0 - cos_i * cos_i);
	
	// Total internal reflection
	if (sin_t_sq > 1.0)
		return (1.0);
	
	cos_t = sqrt(1.0 - sin_t_sq);
	
	// Fresnel equations
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
	t_vec3	refracted_dir;
	t_ray	refracted_ray;
	t_color	refraction_color;
	t_color	final_color;
	double	transparency;
	double	n1;
	double	n2;
	double	cos_i;
	double	fresnel;
	t_vec3	hit_offset;

	// Check if we should calculate refraction
	if (!hit || !hit->material || !scene || depth <= 0)
		return (color_new(0, 0, 0));
	
	transparency = hit->material->transparency;
	if (transparency <= 0.0)
		return (color_new(0, 0, 0));
	
	// Set refractive indices (air = 1.0, material = refraction_index)
	n1 = 1.0; // Air
	n2 = hit->material->refraction_index;
	cos_i = -vec3_dot(ray.direction, hit->normal);
	
	// Calculate refracted ray direction using Snell's law
	refracted_dir = calculate_refraction_ray(ray.direction, hit->normal, n1, n2);
	
	// Check for total internal reflection
	if (vec3_length(refracted_dir) < EPSILON)
		return (color_new(0, 0, 0));
	
	// Create refracted ray slightly offset from surface
	hit_offset = vec3_add(hit->point, vec3_mult(refracted_dir, EPSILON));
	refracted_ray = ray_new(hit_offset, refracted_dir);
	
	// Recursively trace the refracted ray
	refraction_color = raycast(refracted_ray, scene, depth - 1);
	
	// Calculate Fresnel effect
	fresnel = calculate_fresnel(fabs(cos_i), n1, n2);
	
	// Apply transparency and Fresnel effect
	final_color = color_mult(refraction_color, transparency * (1.0 - fresnel));
	
	return (final_color);
}