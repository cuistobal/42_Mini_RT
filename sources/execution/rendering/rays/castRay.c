#include "minirt.h"

//
void	init_hit_values(t_hit *hit)
{
	hit->hit = false;
	hit->distance = 1e30;
	hit->color = set_vec_value(0, 0, 0);
	hit->point = set_vec_value(0, 0, 0);
	hit->normal = set_vec_value(0, 0, 0);
	set_standard_material(&hit->material);
}

/*
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
	t_vec		light_pos;
    t_vec		light_dir;
    t_hit		shadow_hit;

	lights = scene->light;
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
*/

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
//We cast a ray. If it hits an object, we send recursively send another ray to
//find the light source(s). We've put a depth limit for performance purposes.
int	cast_ray(t_scene *scene, t_vec orig, t_vec dir, int depth)
{
	t_hit	hit;
	t_vec	reflectdir;

	init_hit_values(&hit);
	if (depth > MAX_RAY_DEPTH)
		return(0);
	else if (!scene_intersect(scene, orig, dir, &hit))
		return (scene->ambiant->pdata.color); //Add ratio here
	depth++;
	reflectdir = vec_normalized(reflect(dir, hit.normal));
	return (cast_ray(scene, hit.point, reflectdir, depth));
}
