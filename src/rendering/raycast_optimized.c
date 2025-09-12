/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_optimized.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/12 07:54:54 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

// Récupère la couleur d'un pixel (XPM déjà chargé en mémoire)
static t_color xpm_get_pixel(t_texture *tex, int x, int y)
{
    unsigned int    pix;

    pix = tex->data[y * tex->width + x];
    return ((t_color){
        .r = (pix >> 16) & 0xFF,
        .g = (pix >> 8) & 0xFF,
        .b = pix & 0xFF
    });
}

t_color apply_texture(t_hit *hit)
{
    int         x;
    int         y;
    double      u;
    double      v;
    t_texture   *tex;

/*
    if (!hit || !hit->material) // add condition if bump activated
        return hit->material->color;
*/
    tex = (t_texture *)hit->material->texture_addr;
    if (!tex || !tex->data)
        return hit->material->color;
    if (hit->object->type == SPHERE)
    {
        t_vec3 p = vec3_sub(hit->point, hit->object->position);
        p = vec3_normalize(p);
        u = 0.5 + atan2(p.z, p.x) / (2 * M_PI);
        v = 0.5 - asin(p.y) / M_PI;
    }
    else if (hit->object->type == CYLINDER)
    {
        t_vec3 p = vec3_sub(hit->point, hit->object->position);
        u = (atan2(p.z, p.x) + M_PI) / (2 * M_PI);
        v = fmod(p.y / hit->object->height, 1.0);
        if (v < 0) v += 1.0;
    }
    else
    {
        u = fmod(hit->point.x, 1.0);
        v = fmod(hit->point.z, 1.0);
        if (u < 0)
            u += 1.0;
        if (v < 0)
            v += 1.0;
    }
    x = (int)(u * tex->width) % tex->width;
    y = (int)(v * tex->height) % tex->height;
    return (xpm_get_pixel(tex, x, y));
}

static void apply_chess_texture(t_hit *hit, t_color color)
{
    int x;
    int y;
    int z;
    int scale;
     
    (void)color; 
    scale = 1;
    x = (int)floor(hit->point.x / scale);
    y = (int)floor(hit->point.y / scale);
    z = (int)floor(hit->point.z / scale);
    if ((x + y + z) % 2 == 0)
    //hit->material->color = color_new(color.r, color.g, color.b);
        hit->material->color = color_new(255, 255, 255);
    else
        hit->material->color = color_new(0, 0, 0); 
}

/*
** calculate_hit_color - Calculate color for a ray-object intersection
*/
t_color calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
    t_color base_color;
    t_color reflection_color;
    t_color refraction_color;


    if (hit->material->chess == 66)
        apply_chess_texture(hit, hit->material->color);
    hit->material->color = apply_texture(hit);
    base_color = calculate_lighting(hit->point, hit->normal, scene, hit->material);
    reflection_color = calculate_reflection(ray, hit, scene, depth);
    refraction_color = calculate_refraction(ray, hit, scene, depth);
    return color_add(color_add(base_color, reflection_color), refraction_color);
}
