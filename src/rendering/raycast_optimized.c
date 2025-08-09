// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   raycast_optimized.c                                :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
// /*   Updated: 2025/07/30 07:37:53 by cuistobal        ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_hit_color - Calculate color for a ray-object intersection
*/
t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
	t_color	base_color;
	t_color	reflection_color;
	t_color	refraction_color;
	t_color	final_color;

	base_color = calculate_lighting(hit->point, hit->normal, scene,
			hit->material);
	reflection_color = calculate_reflection(ray, hit, scene, depth);
	refraction_color = calculate_refraction(ray, hit, scene, depth);
	final_color = color_add(base_color, reflection_color);
	final_color = color_add(final_color, refraction_color);
	return (color_clamp(final_color));
}

// /*
// ** add_reflection_if_needed - Add reflection to base color if material has it
// */
// static t_color	add_reflection_if_needed(t_color base_color, t_ray ray,
// 	t_hit *hit, t_scene *scene, int depth)
// {
// 	t_color	reflection_color;

// 	if (hit->material->reflection > 0.01 && depth > 1)
// 	{
// 		reflection_color = calculate_reflection(ray, hit, scene, depth);
// 		base_color = color_add(base_color,
// 				color_mult(reflection_color, hit->material->reflection));
// 	}
// 	return (base_color);
// }

// /*
// ** add_refraction_if_needed - Add refraction to base color if material has it
// */
// static t_color	add_refraction_if_needed(t_color base_color, t_ray ray,
// 	t_hit *hit, t_scene *scene, int depth)
// {
// 	t_color	refraction_color;

// 	if (hit->material->transparency > 0.01 && depth > 1)
// 	{
// 		refraction_color = calculate_refraction(ray, hit, scene, depth);
// 		base_color = color_add(base_color,
// 				color_mult(refraction_color, hit->material->transparency));
// 	}
// 	return (base_color);
// }

// /*
// ** raycast_optimized - Optimized ray casting with early termination
// */
// t_color	raycast_optimized(t_ray ray, t_scene *scene, int depth)
// {
// 	t_hit	hit;
// 	t_color	base_color;

// 	if (!scene || depth <= 0)
// 		return (color_new(0, 0, 0));
// 	if (intersect_scene(ray, scene, &hit))
// 	{
// 		base_color = calculate_lighting(hit.point, hit.normal, scene,
// 				hit.material);
// 		base_color = add_reflection_if_needed(base_color, ray, &hit,
// 				scene, depth);
// 		base_color = add_refraction_if_needed(base_color, ray, &hit,
// 				scene, depth);
// 		return (color_clamp(base_color));
// 	}
// 	return (color_new(135, 206, 255));
// }