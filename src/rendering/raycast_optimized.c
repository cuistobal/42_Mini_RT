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
#include <math.h>

/* Safe pixel read (wrap) */
static t_color	xpm_get_pixel_safe(t_texture *tex, int x, int y)
{
    unsigned int	pix;

    if (!tex || !tex->data || tex->width <= 0 || tex->height <= 0)
        return (t_color){ .r = 0, .g = 0, .b = 0 };
    x = x % tex->width;
    if (x < 0)
        x += tex->width;
    y = y % tex->height;
    if (y < 0)
        y += tex->height;
    pix = tex->data[y * tex->width + x];
    return (t_color){ .r = (pix >> 16) & 0xFF,
                      .g = (pix >> 8) & 0xFF,
                      .b = pix & 0xFF };
}

/* UV generation per object type */
static void	get_uv_for_hit(t_hit *hit, double *u, double *v)
{
    t_vec3	p;

    if (!hit || !u || !v)
        return;
    if (hit->object->type == SPHERE)
    {
        p = vec3_sub(hit->point, hit->object->position);
        p = vec3_normalize(p);
        *u = 0.5 + atan2(p.z, p.x) / (2 * M_PI);
        *v = 0.5 - asin(p.y) / M_PI;
        return;
    }
    if (hit->object->type == CYLINDER)
    {
        p = vec3_sub(hit->point, hit->object->position);
        *u = (atan2(p.z, p.x) + M_PI) / (2 * M_PI);
        *v = fmod(p.y / hit->object->height, 1.0);
        if (*v < 0)
            *v += 1.0;
        return;
    }
    *u = fmod(hit->point.x, 1.0);
    *v = fmod(hit->point.z, 1.0);
    if (*u < 0)
        *u += 1.0;
    if (*v < 0)
        *v += 1.0;
}

/* Nearest sample with wrap */
static t_color	sample_texture(t_texture *tex, double u, double v)
{
    int	x;
    int	y;

    if (!tex || !tex->data || tex->width <= 0 || tex->height <= 0)
        return (t_color){ .r = 0, .g = 0, .b = 0 };
    x = (int)floor(u * tex->width);
    y = (int)floor(v * tex->height);
    return (xpm_get_pixel_safe(tex, x, y));
}

/* Chess pattern from UV (returns color, does not mutate material) */
static t_color	chess_color_from_uv(double u, double v, int scale)
{
    int	iu;
    int	iv;

    iu = (int)floor(u * (double)scale);
    iv = (int)floor(v * (double)scale);
    if (((iu + iv) & 1) == 0)
        return (t_color){ .r = 255, .g = 255, .b = 255 };
    return (t_color){ .r = 0, .g = 0, .b = 0 };
}

/* Bump via heightmap (best-effort). Uses MATERIAL_BUMP_ADDR define if available */
static t_vec3	apply_bump_map_if_present(t_hit *hit,
                                         t_vec3 normal,
                                         double u,
                                         double v)
{
    t_texture	*bump;
    t_color	c;
    t_color	cx;
    t_color	cy;
    double	h;
    double	hx;
    double	hy;
    const double	eps = 1.0 / 512.0;
    t_vec3	up;
    t_vec3	tangent;
    t_vec3	bitangent;
    t_vec3	pert;

    if (!hit || !hit->material)
        return (normal);
#ifdef MATERIAL_BUMP_ADDR
    bump = (t_texture *)hit->material->bump_addr;
#else
    bump = NULL;
#endif
    if (!bump || !bump->data)
        return (normal);
    c = sample_texture(bump, u, v);
    cx = sample_texture(bump, u + eps, v);
    cy = sample_texture(bump, u, v + eps);
    h = (c.r + c.g + c.b) / (3.0 * 255.0);
    hx = ((cx.r + cx.g + cx.b) / (3.0 * 255.0)) - h;
    hy = ((cy.r + cy.g + cy.b) / (3.0 * 255.0)) - h;
    up = (fabs(normal.y) < 0.99) ? vec3_new(0, 1, 0) : vec3_new(1, 0, 0);
    tangent = vec3_normalize(vec3_cross(up, normal));
    bitangent = vec3_cross(normal, tangent);
    pert = vec3_add(vec3_mult(tangent, hx * 10.0),
                    vec3_mult(bitangent, hy * 10.0));
    return (vec3_normalize(vec3_add(normal, pert)));
}

/* apply_texture returns color, does not mutate material */
t_color	apply_texture(t_hit *hit)
{
    double	u;
    double	v;
    t_texture	*tex;

    if (!hit || !hit->material)
        return (t_color){ .r = 0, .g = 0, .b = 0 };
    tex = (t_texture *)hit->material->texture_addr;
    if (!tex || !tex->data)
        return (hit->material->color);
    get_uv_for_hit(hit, &u, &v);
    return (sample_texture(tex, u, v));
}

/* calculate_hit_color: do not mutate material. Create temporaries for lighting.
 * Steps: UV -> bump (normal) -> texture/chess (albedo) -> lighting + effects
 */
t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
    t_color	base_color;
    t_color	reflection_color;
    t_color	refraction_color;
    double	u;
    double	v;
    t_vec3	normal;
    t_color	albedo;
    t_material	tmp_mat;
    t_hit	tmp_hit;

    if (!hit || !hit->material || !scene)
        return (color_new(0, 0, 0));
    u = 0.0;
    v = 0.0;
    get_uv_for_hit(hit, &u, &v);
    normal = hit->normal;
    normal = apply_bump_map_if_present(hit, normal, u, v);
    albedo = apply_texture(hit);
    if (hit->material->chess == 66)
    {
        int	scale;

#ifdef MATERIAL_CHESS_SCALE
        scale = hit->material->chess_scale > 0 ? hit->material->chess_scale : 1;
#else
        scale = 1;
#endif
        albedo = chess_color_from_uv(u, v, scale);
    }
    tmp_mat = *hit->material;
    tmp_mat.color = albedo;
    tmp_hit = *hit;
    tmp_hit.material = &tmp_mat;
    tmp_hit.normal = normal;
    base_color = calculate_lighting(tmp_hit.point, tmp_hit.normal, scene, tmp_hit.material);
    reflection_color = calculate_reflection(ray, &tmp_hit, scene, depth);
    refraction_color = calculate_refraction(ray, &tmp_hit, scene, depth);
    return (color_add(color_add(base_color, reflection_color), refraction_color));
}
