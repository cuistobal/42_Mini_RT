/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_ambient_lighting - Calculate ambient lighting contribution
** scene: Scene containing ambient light information
** material: Material properties of the surface
** Returns: Ambient color contribution
*/
static t_color	calculate_ambient_lighting(t_scene *scene, t_material *material)
{
	   t_color material_color;
	   
	   if (!scene || !material)
			   return (color_new(0, 0, 0));
	   material_color = material->color;
	   return color_clamp((t_color){
			   (int)((scene->ambient.r * scene->ambient_ratio * material_color.r) / 255.0),
			   (int)((scene->ambient.g * scene->ambient_ratio * material_color.g) / 255.0),
			   (int)((scene->ambient.b * scene->ambient_ratio * material_color.b) / 255.0)
	   });
}

/*
** calculate_diffuse_lighting - Calculate diffuse lighting contribution from all lights
** point: 3D point where lighting is calculated
** normal: Surface normal at the point (should be normalized)
** scene: Scene containing light sources
** material: Material properties of the surface
** Returns: Diffuse color contribution from all lights
*/
static t_color	calculate_diffuse_lighting(t_vec3 point, t_vec3 normal, t_scene *scene, t_material *material)
{
	t_light	*current_light;
	t_vec3	(light_dir), to_light;
	double	(dot_product), intensity;
	t_color	(light_color), total_diffuse;

	if (!scene || !material)
		return (color_new(0, 0, 0));
	total_diffuse = color_new(0, 0, 0);
	current_light = scene->lights;
	while (current_light)
	{
		to_light = vec3_sub(current_light->position, point);
		light_dir = vec3_normalize(to_light);
		dot_product = vec3_dot(normal, light_dir);
		if (dot_product > 0.0)
		{
			if (!is_in_shadow(point, current_light, scene))
			{
				intensity = dot_product * current_light->intensity;
				light_color = current_light->color;
				total_diffuse = color_add(total_diffuse, (t_color){
					   (int)((material->color.r * light_color.r * intensity) / 255.0),
					   (int)((material->color.g * light_color.g * intensity) / 255.0),
					   (int)((material->color.b * light_color.b * intensity) / 255.0)});
			}
		}
		current_light = current_light->next;
	}
	return (color_clamp(total_diffuse));
}

/*
** calculate_lighting - Main lighting calculation function
** point: 3D point where lighting is calculated
** normal: Surface normal at the point
** scene: Scene containing lights and ambient information
** material: Material properties of the surface
** Returns: Final color after lighting calculations
*/
t_color	calculate_lighting(t_vec3 point, t_vec3 normal, t_scene *scene, t_material *material)
{
	t_color	ambient_color;
	t_color	diffuse_color;
	
	if (!scene || !material)
		return (color_new(0, 0, 0));
	ambient_color = calculate_ambient_lighting(scene, material);
	diffuse_color = calculate_diffuse_lighting(point, normal, scene, material);
	return (color_add(ambient_color, diffuse_color));
}

/*
** is_in_shadow - Check if a point is in shadow from a specific light
** point: 3D point to test for shadows
** light: Light source to test occlusion from
** scene: Scene containing objects that can cast shadows
** Returns: 1 if point is in shadow, 0 if illuminated
*/
int	is_in_shadow(t_vec3 point, t_light *light, t_scene *scene)
{
	t_ray	shadow_ray;
	t_vec3	to_light;
	t_vec3	shadow_origin;
	double	light_distance;
	t_hit	shadow_hit;

	if (!light || !scene)
		return (0);
	
	// Calculate direction from point to light
	to_light = vec3_sub(light->position, point);
	light_distance = vec3_length(to_light);
	
	// Create shadow ray slightly offset from surface to avoid self-intersection
	shadow_origin = vec3_add(point, vec3_mult(vec3_normalize(to_light), EPSILON));
	shadow_ray = ray_new(shadow_origin, vec3_normalize(to_light));
	
	// Test for intersection between point and light
	if (intersect_scene(shadow_ray, scene, &shadow_hit))
	{
		// If intersection is closer than light, point is in shadow
		if (shadow_hit.t < light_distance - EPSILON)
			return (1);
	}
	
	return (0);
}