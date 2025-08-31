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

/*t_color calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
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
}*/
#include "../includes/minirt.h"
#include <math.h>

t_color xpm_get_color(t_texture *tex, int x, int y)
{
    unsigned int    pix;
    t_color         c;

    pix = xpm_get_pixel(tex, x, y);
    c.r = (pix >> 16) & 0xFF;
    c.g = (pix >> 8) & 0xFF;
    c.b = pix & 0xFF;
    return (c);
}


t_color texture_get_color(t_texture *tex, double u, double v)
{
    int x;
    int y;
    unsigned int pix;
    int r;
    int g;
    int b;

    if (!tex || !tex->data || tex->width <= 0 || tex->height <= 0)
        return (color_new(255, 0, 255)); // couleur debug si pb

    /* wrap des UV -> toujours en [0,1) */
    if (u < 0.0)
        u = u - floor(u);
    if (v < 0.0)
        v = v - floor(v);

    /* convertit en indices pixels */
    x = (int)floor(u * (double)(tex->width - 1));
    y = (int)floor(v * (double)(tex->height - 1));

    while (x < 0)
        x = x + tex->width;
    while (x >= tex->width)
        x = x - tex->width;
    while (y < 0)
        y = y + tex->height;
    while (y >= tex->height)
        y = y - tex->height;

    pix = tex->data[y * tex->width + x];

    r = (int)((pix >> 16) & 0xFF);
    g = (int)((pix >> 8) & 0xFF);
    b = (int)(pix & 0xFF);

    return (color_new(r, g, b));
}



t_color calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
    t_color base_color;
    t_color reflection_color;
    t_color refraction_color;

    // --- Chessboard ---
    if (hit->material->use_chessboard)
    {
        int scale = 1;
        int x = (int)floor(hit->point.x / scale);
        int y = (int)floor(hit->point.y / scale);
        int z = (int)floor(hit->point.z / scale);

        if ((x + y + z) % 2 == 0)
            hit->material->color = color_new(255, 255, 255);
        else
            hit->material->color = color_new(0, 0, 0);
    }

    // --- Texture diffuse ---
    else if (hit->material->use_texture && hit->material->texture)
    {
        hit->material->color = texture_get_color(hit->material->texture,
                                                 hit->uv.u, hit->uv.v);
    }

    // --- Éclairage standard ---
    base_color = calculate_lighting(hit->point, hit->normal, scene, hit->material);
    reflection_color = calculate_reflection(ray, hit, scene, depth);
    refraction_color = calculate_refraction(ray, hit, scene, depth);

    return color_add(color_add(base_color, reflection_color), refraction_color);
}
