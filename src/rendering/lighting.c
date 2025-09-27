/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/27 07:36:37 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_ambient_lighting - Calculate ambient lighting contribution
** scene: Scene containing ambient light information
** material: Material properties of the surface
** Returns: Ambient color contribution
*/
static t_color	calculate_ambient_lighting(t_scene *scene, t_material *mat)
{
	return ((t_color){
		(int)((scene->ambient.r * scene->ambient_ratio
			* mat->color.r) / 255.0),
		(int)((scene->ambient.g * scene->ambient_ratio
			* mat->color.g) / 255.0),
		(int)((scene->ambient.b * scene->ambient_ratio
			* mat->color.b) / 255.0)
	});
}

/*
** calculate_diffuse_lighting - Calculate diffuse lighting contribution from 
** all light points: 3D point where lighting is calculated
** normal: Surface normal at the point (should be normalized)
** scene: Scene containing light sources
** material: Material properties of the surface
** Returns: Diffuse color contribution from all lights
*/
static inline t_color	append_diffuse(t_color total_diffuse,
		t_material *material, t_light *light, double intensity)
{
	return ((t_color)color_add(total_diffuse, (t_color){
			(int)((material->color.r * light->color.r * intensity) / 255.0),
		(int)((material->color.g * light->color.g * intensity) / 255.0),
		(int)((material->color.b * light->color.b * intensity) / 255.0)
		}));
}

static void	set_lightning(t_lightning *l, t_scene *scene)
{
	l->dot = 0.0;
	l->dist = 0.0;
	l->total_diffuse = color_new(0, 0, 0);
	l->current_light = scene->lights;
	l->to_light = (t_vec3){0, 0, 0};
	l->dir_to_light = (t_vec3){0, 0, 0};
}

static t_color	calculate_diffuse_lighting(t_vec3 point, t_vec3 normal,
		t_scene *scene, t_material *material)
{
	t_lightning	l;

	set_lightning(&l, scene);
	while (l.current_light)
	{
		l.to_light = vec3_sub(l.current_light->position, point);
		l.dir_to_light = vec3_normalize(l.to_light);
		l.dist = vec3_length(l.to_light);
		l.dot = vec3_dot(normal, l.dir_to_light);
		if (l.dot > 0.0
			&& !is_in_shadow_with_dir(point, l.dir_to_light, l.dist, scene))
			l.total_diffuse = append_diffuse(l.total_diffuse, material,
					l.current_light, l.dot * l.current_light->intensity);
		l.current_light = l.current_light->next;
	}
	return (l.total_diffuse);
}

/*
** calculate_lighting - Main lighting calculation function
** point: 3D point where lighting is calculated
** normal: Surface normal at the point
** scene: Scene containing lights and ambient information
** material: Material properties of the surface
** Returns: Final color after lighting calculations
*/
t_color	calculate_lighting(t_vec3 point, t_vec3 normal, t_scene *scene,
		t_material *material)
{
	return (color_add(calculate_ambient_lighting(scene, material),
			calculate_diffuse_lighting(point, normal, scene, material)));
}
