/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_normed_raytracer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/07 14:41:10 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#ifndef AMBIANT_LIGHTNING
# define AMBIANT_LIGHTNING 0.5
#endif

typedef struct s_vec
{
	float	x;
	float	y;
	float	z;
}	t_vec;

typedef struct s_material
{
	float	refractive_index;
	float	albedo[4];
	t_vec	diffuse_color;
	float	specular_exponent;
}	t_material;

typedef struct s_sphere
{
	t_vec		center;
	float		radius;
	t_material	material;
}	t_sphere;

typedef struct s_hit
{
	bool		hit;
	float		distance;
	t_vec		point;
	t_vec		normal;
	t_material	material;
}	t_hit;

typedef struct s_obj	t_obj;
typedef bool			(*t_intersect_fn)(t_obj *self, t_vec orig, t_vec dir, t_hit *hit);

struct s_obj
{
	char			type;
	void			*data;
	t_intersect_fn	intersect;
	t_obj			*next;
};

typedef struct s_scene
{
	t_obj	*objects;
	t_vec	*lights;
	size_t	light_count;
}	t_scene;

t_vec	ft_vec3_add(t_vec a, t_vec b)
{
	return ((t_vec){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec	ft_vec3_sub(t_vec a, t_vec b)
{
	return ((t_vec){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec	ft_vec3_scale(t_vec v, float s)
{
	return ((t_vec){v.x * s, v.y * s, v.z * s});
}

float	ft_vec3_dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float	ft_vec3_norm(t_vec v)
{
	return (sqrtf(ft_vec3_dot(v, v)));
}

t_vec	ft_vec3_normalized(t_vec v)
{
	float	n;

	n = ft_vec3_norm(v);
	return (ft_vec3_scale(v, 1.0f / n));
}

t_vec	ft_vec3_negate(t_vec v)
{
	return ((t_vec){-v.x, -v.y, -v.z});
}

t_vec	ft_vec3_cross(t_vec a, t_vec b)
{
	t_vec	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

bool	ft_sphere_intersect_calc(t_sphere *s, t_vec orig, t_vec dir)
{
	t_vec	l;
	float	tca;
	float	d2;
	float	r2;

	l = ft_vec3_sub(s->center, orig);
	tca = ft_vec3_dot(l, dir);
	d2 = ft_vec3_dot(l, l) - tca * tca;
	r2 = s->radius * s->radius;
	if (d2 > r2)
		return (false);
	return (true);
}

float	ft_sphere_intersect_dist(t_sphere *s, t_vec orig, t_vec dir)
{
	t_vec	l;
	float	tca;
	float	d2;
	float	r2;
	float	thc;

	l = ft_vec3_sub(s->center, orig);
	tca = ft_vec3_dot(l, dir);
	d2 = ft_vec3_dot(l, l) - tca * tca;
	r2 = s->radius * s->radius;
	thc = sqrtf(r2 - d2);
	return (tca - thc);
}

bool	get_distance(float *distance, float t0, float t1)
{
	if (t0 > 0)
		*distance = t0;
	else
		*distance = t1;
	return (*distance > 0);
}

bool	ft_sphere_intersect(t_obj *self, t_vec orig, t_vec dir, t_hit *hit)
{
	t_sphere	*s;
	float		t0;
	float		t1;
	float		distance;

	s = (t_sphere *)self->data;
	if (!ft_sphere_intersect_calc(s, orig, dir))
		return (false);
	t0 = ft_sphere_intersect_dist(s, orig, dir);
	t1 = t0 + 2 * sqrtf(s->radius * s->radius - 
		(ft_vec3_dot(ft_vec3_sub(s->center, orig), ft_vec3_sub(s->center, orig)) - 
		ft_vec3_dot(ft_vec3_sub(s->center, orig), dir) * ft_vec3_dot(ft_vec3_sub(s->center, orig), dir)));
	if (!get_distance(&distance, t0, t1))
		return (false);
	hit->hit = true;
	hit->distance = distance;
	hit->point = ft_vec3_add(orig, ft_vec3_scale(dir, distance));
	hit->normal = ft_vec3_normalized(ft_vec3_sub(hit->point, s->center));
	hit->material = s->material;
	return (true);
}

t_obj	*ft_create_sphere(t_vec center, float radius, t_material mat)
{
	t_obj		*obj;
	t_sphere	*s;

	obj = malloc(sizeof(t_obj));
	s = malloc(sizeof(t_sphere));
	if (!obj || !s)
		return (NULL);
	*s = (t_sphere){center, radius, mat};
	obj->type = 's';
	obj->data = s;
	obj->intersect = ft_sphere_intersect;
	obj->next = NULL;
	return (obj);
}

bool	ft_scene_intersect(t_scene *scene, t_vec orig, t_vec dir, t_hit *closest_hit)
{
	bool	found;
	float	min_dist;
	t_obj	*obj;
	t_hit	hit;

	found = false;
	min_dist = 1e30;
	obj = scene->objects;
	while (obj)
	{
		hit = (t_hit){0};
		if (obj->intersect(obj, orig, dir, &hit) && hit.distance < min_dist)
		{
			min_dist = hit.distance;
			*closest_hit = hit;
			found = true;
		}
		obj = obj->next;
	}
	return (found);
}

t_vec	ft_reflect(t_vec i, t_vec n)
{
	return (ft_vec3_sub(i, ft_vec3_scale(n, 2.0f * ft_vec3_dot(i, n))));
}

float	ft_refract_helper(t_vec i, t_vec n, float eta_i, float eta_t)
{
	float	cosi;
	float	eta;
	float	k;

	cosi = -fmaxf(-1.0f, fminf(1.0f, ft_vec3_dot(i, n)));
	if (cosi < 0)
		return (-1);
	eta = eta_i / eta_t;
	k = 1 - eta * eta * (1 - cosi * cosi);
	return (k);
}

t_vec	ft_refract(t_vec i, t_vec n, float eta_t, float eta_i)
{
	float	cosi;
	float	eta;
	float	k;

	cosi = -fmaxf(-1.0f, fminf(1.0f, ft_vec3_dot(i, n)));
	if (cosi < 0)
		return (ft_refract(i, ft_vec3_negate(n), eta_i, eta_t));
	eta = eta_i / eta_t;
	k = ft_refract_helper(i, n, eta_i, eta_t);
	if (k < 0)
		return ((t_vec){1, 0, 0});
	return (ft_vec3_add(ft_vec3_scale(i, eta), 
		ft_vec3_scale(n, eta * cosi - sqrtf(k))));
}

bool	ft_check_shadow(t_scene *scene, t_vec point, t_vec light_pos)
{
	t_vec	light_dir;
	t_hit	shadow_hit;
	float	light_dist;
	float	shadow_dist;

	light_dir = ft_vec3_normalized(ft_vec3_sub(light_pos, point));
	shadow_hit = (t_hit){0};
	if (!ft_scene_intersect(scene, point, light_dir, &shadow_hit))
		return (false);
	light_dist = ft_vec3_norm(ft_vec3_sub(light_pos, point));
	shadow_dist = ft_vec3_norm(ft_vec3_sub(shadow_hit.point, point));
	return (shadow_dist < light_dist);
}

void	ft_calculate_lighting(t_scene *scene, t_hit *hit, t_vec dir, float *diffuse, float *specular)
{
	size_t	i;
	t_vec	light_dir;
	t_vec	reflect_dir;

	*diffuse = 0;
	*specular = 0;
	i = 0;
	while (i < scene->light_count)
	{
		if (ft_check_shadow(scene, hit->point, scene->lights[i]))
		{
			i++;
			continue;
		}
		light_dir = ft_vec3_normalized(ft_vec3_sub(scene->lights[i], hit->point));
		reflect_dir = ft_reflect(ft_vec3_negate(light_dir), hit->normal);
		*diffuse += fmaxf(0.0f, ft_vec3_dot(light_dir, hit->normal));
		*specular += powf(fmaxf(0.0f, -ft_vec3_dot(reflect_dir, dir)), 
			hit->material.specular_exponent);
		i++;
	}
}


t_vec	ft_calculate_final_color(t_hit *hit, t_vec reflect_color, \
		t_vec refract_color, t_scene *scene, t_vec dir);

//SHould returnt he ambiant lightning if no intersection / depth reached
t_vec	ft_cast_ray(t_scene *scene, t_vec orig, t_vec dir, int depth)
{
	t_hit	hit;
	t_vec	reflect_dir;
	t_vec	refract_dir;
	t_vec	reflect_color;
	t_vec	refract_color;

	if (depth > 4)
		//return ((t_vec){0.2, 0.7, 0.8});
		return ((t_vec){AMBIANT_LIGHTNING, AMBIANT_LIGHTNING, AMBIANT_LIGHTNING});
	hit = (t_hit){0};
	if (!ft_scene_intersect(scene, orig, dir, &hit))
		return ((t_vec){AMBIANT_LIGHTNING, AMBIANT_LIGHTNING, AMBIANT_LIGHTNING});
		//return ((t_vec){0.2, 0.7, 0.8});
	reflect_dir = ft_vec3_normalized(ft_reflect(dir, hit.normal));
	refract_dir = ft_vec3_normalized(ft_refract(dir, hit.normal, 
		hit.material.refractive_index, 1.0f));
	reflect_color = ft_cast_ray(scene, hit.point, reflect_dir, depth + 1);
	refract_color = ft_cast_ray(scene, hit.point, refract_dir, depth + 1);
	return ((t_vec)ft_calculate_final_color(&hit, reflect_color, refract_color, scene, dir));
}

t_vec	ft_calculate_final_color(t_hit *hit, t_vec reflect_color, t_vec refract_color, 
	t_scene *scene, t_vec dir)
{
	float	diffuse_intensity;
	float	specular_intensity;
	t_vec	diffuse;
	t_vec	specular;
	t_vec	result;

	ft_calculate_lighting(scene, hit, dir, &diffuse_intensity, &specular_intensity);
	diffuse = ft_vec3_scale(hit->material.diffuse_color, 
		diffuse_intensity * hit->material.albedo[0]);
	specular = ft_vec3_scale((t_vec){1.0, 1.0, 1.0}, 
		specular_intensity * hit->material.albedo[1]);
	result = ft_vec3_add(diffuse, specular);
	result = ft_vec3_add(result, ft_vec3_scale(reflect_color, hit->material.albedo[2]));
	result = ft_vec3_add(result, ft_vec3_scale(refract_color, hit->material.albedo[3]));
	return (result);
}

//
void	ft_init_materials(t_material *ivory, t_material *glass, 
	t_material *red_rubber, t_material *mirror, t_material *none, t_material *invisible)
{
	*ivory = (t_material){1.0, {0.9, 0.5, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.0};
	*glass = (t_material){1.5, {0.0, 0.9, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.0};
	*red_rubber = (t_material){1.0, {1.4, 0.3, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.0};
	*mirror = (t_material){1.0, {0.0, 16.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.0};
	*none = (t_material){1.0, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 1.0};
	*invisible = (t_material){AMBIANT_LIGHTNING, {AMBIANT_LIGHTNING, AMBIANT_LIGHTNING, AMBIANT_LIGHTNING, AMBIANT_LIGHTNING}, {AMBIANT_LIGHTNING, AMBIANT_LIGHTNING, AMBIANT_LIGHTNING}, AMBIANT_LIGHTNING};
}

void	ft_init_scene(t_scene *scene, t_material ivory, t_material glass, 
	t_material red_rubber, t_material mirror)
{
	t_vec	lights[3];

	scene->objects = ft_create_sphere((t_vec){-3, 0, -16}, 2, ivory);
	scene->objects->next = ft_create_sphere((t_vec){-1.0, -1.5, -12}, 2, glass);
	scene->objects->next->next = ft_create_sphere((t_vec){1.5, -0.5, -18}, 3, red_rubber);
	scene->objects->next->next->next = ft_create_sphere((t_vec){7, 5, -18}, 4, mirror);
	lights[0] = (t_vec){-20, 20, 20};
	lights[1] = (t_vec){30, 50, -25};
	lights[2] = (t_vec){30, 20, 30};
	scene->lights = lights;
	scene->light_count = 3;
}

void	ft_render_pixel(t_scene *scene, t_vec *framebuffer, int i, int j, 
	int width, int height, float fov)
{
	float	dir_x;
	float	dir_y;
	float	dir_z;
	t_vec	dir;

	dir_x = (i + 0.5f) - width / 2.0f;
	dir_y = -(j + 0.5f) + height / 2.0f;
	dir_z = -height / (2.0f * tanf(fov / 2.0f));
	dir = ft_vec3_normalized((t_vec){dir_x, dir_y, dir_z});
	framebuffer[j * width + i] = ft_cast_ray(scene, (t_vec){0, 0, 0}, dir, 0);
}

void	ft_render_frame(t_scene *scene, t_vec *framebuffer, int width, int height)
{
	int		i;
	int		j;
	float	fov;

	//fov = scene->camera.u_type.camera.fov;
	fov = 1.0f;
	j = 0;
	while (j < height)
	{
		i = 0;
		while (i < width)
		{
			ft_render_pixel(scene, framebuffer, i, j, width, height, fov);
			i++;
		}
		j++;
	}
}

void	ft_write_ppm(t_vec *framebuffer, int width, int height)
{
	FILE	*ofs;
	int		i;
	t_vec	color;
	float	max_val;

	ofs = fopen("out.ppm", "wb");
	if (!ofs)
		return ;
	fprintf(ofs, "P6\n%d %d\n255\n", width, height);
	i = 0;
	while (i < width * height)
	{
		color = framebuffer[i];
		max_val = fmaxf(1.0f, fmaxf(color.x, fmaxf(color.y, color.z)));
		fwrite(&(unsigned char){(unsigned char)(255 * color.x / max_val)}, 1, 1, ofs);
		fwrite(&(unsigned char){(unsigned char)(255 * color.y / max_val)}, 1, 1, ofs);
		fwrite(&(unsigned char){(unsigned char)(255 * color.z / max_val)}, 1, 1, ofs);
		i++;
	}
	fclose(ofs);
}

void	ft_cleanup_scene(t_scene *scene)
{
	t_obj	*obj;
	t_obj	*next;

	obj = scene->objects;
	while (obj)
	{
		next = obj->next;
		free(obj->data);
		free(obj);
		obj = next;
	}
}

int	main(void)
{
	t_material	ivory;
	t_material	glass;
	t_material	red_rubber;
	t_material	mirror;
	t_material	none;
	t_material	invisible;
	t_scene		scene;
	t_vec		*framebuffer;
	int			width;
	int			height;

	width = 1024;
	height = 768;
	ft_init_materials(&ivory, &glass, &red_rubber, &mirror, &none, &invisible);
	ft_init_scene(&scene, invisible, invisible, invisible, mirror);
	framebuffer = malloc(width * height * sizeof(t_vec));
	if (!framebuffer)
		return (1);
	ft_render_frame(&scene, framebuffer, width, height);
	ft_write_ppm(framebuffer, width, height);
	free(framebuffer);
	ft_cleanup_scene(&scene);
	return (0);
}
