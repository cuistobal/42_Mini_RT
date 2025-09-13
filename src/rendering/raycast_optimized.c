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

// Local constant colors for chess pattern
static const t_color	g_colors[2] = {
    (t_color){ .r = 0, .g = 0, .b = 0 },
    (t_color){ .r = 255, .g = 255, .b = 255 }
};

// Allows color blending in cases when we have both texture and chess pattern
static inline t_color color_lerp(t_color a, t_color b, double t)
{
    return ((t_color){
        .r = (int)(a.r * (1.0 - t) + b.r * t),
        .g = (int)(a.g * (1.0 - t) + b.g * t),
        .b = (int)(a.b * (1.0 - t) + b.b * t)
    });
}

/* Safe pixel read (wrap) */
static t_color	xpm_get_pixel_safe(t_texture *tex, int x, int y)
{
    unsigned int	pix;

    if (!tex || !tex->data || tex->width <= 0 || tex->height <= 0)
        return (t_color){ .r = 0, .g = 0, .b = 0 };
    x = x % tex->width;
    x += tex->width * (x < 0);
    y = y % tex->height;
    y += tex->height * (y < 0);
    pix = tex->data[y * tex->width + x];
    return (t_color){ .r = (pix >> 16) & 0xFF,
                      .g = (pix >> 8) & 0xFF,
                      .b = pix & 0xFF };
}

/* UV generation per object type */
static void	get_uv_for_hit(t_hit *hit, double *u, double *v)
{
    t_vec3	p;

    if (hit->object->type == SPHERE)
    {
        p = vec3_normalize(vec3_sub(hit->point, hit->object->position));
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
    int		iu;
    int		iv;

    iu = (int)floor(u * (double)scale);
    iv = (int)floor(v * (double)scale);
    return (g_colors[(iu + iv) & 1]);
}

/* Bump via heightmap (best-effort). Uses MATERIAL_texture_addr define if available */
static t_vec3	apply_bump_map_if_present(t_hit *hit, t_vec3 normal, double u, double v)
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

    bump = NULL;
    if (hit->material->texture_addr != NULL)
        bump = (t_texture *)hit->material->texture_addr;
    if (!bump || !bump->data)
        return (normal);
    c = sample_texture(bump, u, v);
    cx = sample_texture(bump, u + eps, v);
    cy = sample_texture(bump, u, v + eps);
    h = (c.r + c.g + c.b) / (3.0 * 255.0);
    hx = ((cx.r + cx.g + cx.b) / (3.0 * 255.0)) - h;
    hy = ((cy.r + cy.g + cy.b) / (3.0 * 255.0)) - h;
    if (fabs(normal.y) < 0.99)
        up = vec3_new(0, 1, 0);
    else
        up = vec3_new(1, 0, 0);
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

    tex = (t_texture *)hit->material->texture_addr;
    if (!tex || !tex->data)
        return (hit->material->color);
    get_uv_for_hit(hit, &u, &v);
    return (sample_texture(tex, u, v));
}

/* Prepare a temporary hit and material for lighting: computes UV, applies bump,
 * samples texture, blends with chess if present, and fills out_mat/out_hit.
 */
static void	prepare_hit_for_lighting(t_hit *hit, t_hit *out_hit, t_material *out_mat, double blend)
{
    double	u;
    double	v;
    t_vec3	normal;
    t_color	tex_col;
    t_color	albedo;

    get_uv_for_hit(hit, &u, &v);
    normal = apply_bump_map_if_present(hit, hit->normal, u, v);
    tex_col = apply_texture(hit);
    albedo = tex_col;
    if (hit->material->chess > 0)
        albedo = color_lerp(tex_col, chess_color_from_uv(u, v, hit->material->chess), blend);
    *out_mat = *hit->material;
    out_mat->color = albedo;
    *out_hit = *hit;
    out_hit->material = out_mat;
    out_hit->normal = normal;
}

/* calculate_hit_color: do not mutate material. Create temporaries for lighting.
 * Steps: UV -> bump (normal) -> texture/chess (albedo) -> lighting + effects
 */
t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth)
{
    t_hit	    tmp_hit;
    t_material	tmp_mat;
    t_color	    base_color;
    t_color	    reflection_color;
    t_color	    refraction_color;

    prepare_hit_for_lighting(hit, &tmp_hit, &tmp_mat, 0.5);
    base_color = calculate_lighting(tmp_hit.point, tmp_hit.normal, scene, tmp_hit.material);
    reflection_color = calculate_reflection(ray, &tmp_hit, scene, depth);
    refraction_color = calculate_refraction(ray, &tmp_hit, scene, depth);
    return (color_add(color_add(base_color, reflection_color), refraction_color));
}
