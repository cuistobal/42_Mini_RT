#include "minirt.h"

//
void	init_hit_values(t_hit *hit)
{
	hit->hit = false;
	hit->distance = 1e30;
	hit->point = set_vec_value(0, 0, 0);
	hit->normal = set_vec_value(0, 0, 0);
	set_standard_material(&hit->material);
}

//Returns the sum of all the vectors contained in our computed buffer. Hence,
//we get a vector that represents the light status of our ray;
static t_vec	cast_ray_return(t_vec buffer[])
{
	t_vec	sumof1;
	t_vec	sumof2;

	sumof1 = vec_add(buffer[DIFFUSE], buffer[SPECULAR]);
	sumof2 = vec_add(buffer[REFLECTION], buffer[REFRACTION]);
	return (vec_add(sumof1, sumof2));
}

bool	handle_multiple_lights_helper(t_vec lpos, t_hit	shadow_hit, t_hit hit)
{
	float	ln;
	float	sn;
	t_vec	light;
	t_vec	shadow;

	light = vec_sub(lpos, hit.point);
	ln = vec_norm(light);
	shadow = vec_sub(shadow_hit.point, hit.point);
	sn = vec_norm(shadow);
	return (sn < ln);
}

//
static float compute_shadow_light_index(t_vec dir, t_vec light_dir, t_hit hit)
{
	float	dot;
	float	max;	
	t_vec	reflection;
	t_vec	negate_light_dir;

	negate_light_dir = vec_negate(light_dir);
	reflection = reflect(negate_light_dir, hit.normal);
	dot = -vec_dot(reflection, dir); 
	max = fmaxf(0.0f, dot);	
	return (powf(max, hit.material.specular_exponent));
}

//
static void	handle_multiple_lights(t_scene *scene, t_hit hit, t_vec dir, float scales[4])
{
	t_object	*lights;
	t_vec		indices;
	t_vec		light_pos;
    t_vec		light_dir;
    t_hit		shadow_hit;

	lights = scene->light;
	indices = set_vec_value(0, 0, 0);
	while (lights)
	{
		init_hit_values(&shadow_hit);
		light_pos = lights->u_type.light.light_position;
        light_dir = vec_normalized(vec_sub(light_pos, hit.point));	
        if (scene_intersect(scene, hit.point, light_dir, &shadow_hit) &&
			handle_multiple_lights_helper(light_pos, shadow_hit, hit))
            continue;
        scales[DLI] += fmaxf(0.0f, vec_dot(light_dir, hit.normal));
        scales[SLI] += compute_shadow_light_index(dir, light_dir, hit);
		lights = lights->next;
    }
}

//dli -> diffuse light intensity
//sli -> specular light intensity
//
static t_vec	handle_impact(t_scene *scene, t_vec buffer[], t_vec dir, t_hit hit)
{
	t_vec	buff[4];
	float	scales[4];

	scales[0] = 1;	
	scales[1] = 1;	
	scales[2] = 1;	
	scales[3] = 1;
	handle_multiple_lights(scene, hit, dir, scales);
	if (hit.material.set)
	{
		scales[DLI] = scales[DLI] * hit.material.albedo[0];
		scales[SLI] = scales[SLI] * hit.material.albedo[1];
		scales[2] = hit.material.albedo[2];
		scales[3] = hit.material.albedo[3];
	}
    buff[DIFFUSE] = vec_scale(hit.material.diffuse_color, scales[DLI]);
    buff[SPECULAR] = vec_scale((t_vec)set_vec_value(1, 1, 1), scales[SLI]);
    buff[REFLECTION] = vec_scale(buffer[REFLECTCOLOR], scales[2]);
   	buff[REFRACTION] = vec_scale(buffer[REFRACTCOLOR], scales[3]);
	return ((t_vec)(cast_ray_return(buff)));
}

//
bool	scene_intersect(t_scene *scene, t_vec orig, t_vec dir, \
		t_hit *closest_hit)
{
	float		min;
    t_hit		hit;
    bool		found;
	t_object	*current;

    min = 1e30;
	found = false;
//This is where we should use the BVH instead of looping through all objects
	current = scene->objects;
	while (current)
	{
		init_hit_values(&hit);
		if (current->methods.intersect(current, orig, dir, &hit))
		{
			if (hit.distance < min)
			{
            	min = hit.distance;
            	*closest_hit = hit;
            	found = true;
        	}
		}
		current = current->next;
    }
    return (found);
}

// 0 ->	diffuse
// 1 -> specular
// 2 -> reflection
// 3 -> refraction
//
//We cast a ray. If it hits an object, we make also check for reflection && 
//refraction. We've put a depth limit for performance purposes.
t_vec	cast_ray(t_scene *scene, t_vec orig, t_vec dir, int depth)
{
	t_hit	hit;
	t_vec	buffer[4];

	init_hit_values(&hit);
	if (depth > MAX_RAY_DEPTH || !scene_intersect(scene, orig, dir, &hit))
		return ((t_vec)set_vec_value(0.2, 0.7, 0.8));
	depth++;
	buffer[REFLECTDIR] = vec_normalized(reflect(dir, hit.normal));
	buffer[REFRACTDIR] = vec_normalized(refract(dir, hit.normal, \
					hit.material.refractive_index, 1.0f)) ;
	buffer[REFLECTCOLOR] = cast_ray(scene, hit.point, buffer[REFLECTDIR], depth);
	buffer[REFRACTCOLOR] = cast_ray(scene, hit.point, buffer[REFRACTDIR], depth);
	return (handle_impact(scene, buffer, dir, hit));
}
