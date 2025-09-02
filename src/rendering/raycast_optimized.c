// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
/*   raycast_optimized.c                                :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/22 15:45:18 by chrleroy         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** calculate_hit_color - Calculate color for a ray-object intersection
*/
<<<<<<< HEAD
t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
	t_color	base_color;
	t_color	reflection_color;
	t_color	refraction_color;
	
	base_color = calculate_lighting(hit->point, hit->normal, scene, \
			hit->material);
	reflection_color = calculate_reflection(ray, hit, scene, depth);
	refraction_color = calculate_refraction(ray, hit, scene, depth);
	return (color_add(color_add(base_color, reflection_color), \
				refraction_color));
=======
// t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
// {
// 	t_color	base_color;
// 	t_color	reflection_color;
// 	t_color	refraction_color;

// 	base_color = calculate_lighting(hit->point, hit->normal, scene,
// 			hit->material);
// 	reflection_color = calculate_reflection(ray, hit, scene, depth);
// 	refraction_color = calculate_refraction(ray, hit, scene, depth);
// 	return (color_add(color_add(base_color, reflection_color), refraction_color));
// }

t_color calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
    t_color base_color;
    t_color reflection_color;
    t_color refraction_color;

    int scale = 1;
    int x = (int)floor(hit->point.x / scale);
    int y = (int)floor(hit->point.y / scale);
    int z = (int)floor(hit->point.z / scale);

    if ((x + y + z) % 2 == 0)
        hit->material->color = color_new(255, 255, 255);
    else
        hit->material->color = color_new(0, 0, 0);       

    base_color = calculate_lighting(hit->point, hit->normal, scene, hit->material);
    reflection_color = calculate_reflection(ray, hit, scene, depth);
    refraction_color = calculate_refraction(ray, hit, scene, depth);

    return color_add(color_add(base_color, reflection_color), refraction_color);
>>>>>>> merge/normed_thread
}
