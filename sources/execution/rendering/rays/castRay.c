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

//Returns the sum of all the vectors contained in our computed buffer. Hence,
//we get a vector that represents the light status of our ray;
static t_vec	cast_ray_return(t_vec buffer[])
{
    // Add all components
    t_vec result = vec_add(
        vec_add(buffer[DIFFUSE], buffer[SPECULAR]),
        vec_add(buffer[REFLECTION], buffer[REFRACTION])
    );
    
    // Clamp to avoid over-bright areas
    return (clamp_color(result));
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

//dli -> diffuse light intensity
//sli -> specular light intensity
//
static t_vec	handle_impact(t_scene *scene, t_vec buffer[], t_vec dir, t_hit hit)
{
    t_vec	buff[4];
    float	scales[4];

    scales[DLI] = 0.0f;    // Diffuse light intensity
    scales[SLI] = 0.0f;    // Specular light intensity
    scales[2] = 0.2f;      // Reflection intensity
    scales[3] = 0.0f;      // Refraction intensity (if used)
    
    handle_multiple_lights(scene, hit, dir, scales);
    
    // Ambient light (base visibility)
    t_vec ambient = vec_scale(hit.material.diffuse_color, 0.1f);
    
    // Diffuse component
    buff[DIFFUSE] = vec_scale(hit.material.diffuse_color, scales[DLI]);
    
    // Specular component (white highlight, but not too bright)
    buff[SPECULAR] = vec_scale(set_vec_value(0.7f, 0.7f, 0.7f), scales[SLI] * hit.material.specular_exponent * 0.1f);
    
    // Reflection
    buff[REFLECTION] = vec_scale(buffer[REFLECTCOLOR], scales[2] * hit.material.reflectivity);
    
    // Add ambient
    buff[DIFFUSE] = vec_add(buff[DIFFUSE], ambient);
    
    return cast_ray_return(buff);
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
	if (depth > MAX_RAY_DEPTH)
		return((t_vec)orig);
	else if (!scene_intersect(scene, orig, dir, &hit))
		return ((t_vec)set_vec_value(0.2, 0.7, 0.8));
/*
	else	
		return ((t_vec)hit.color);
	//	return (VECTORIZED AMBIENT LIGHTNING);
*/
	depth++;
	buffer[REFLECTDIR] = vec_normalized(reflect(dir, hit.normal));
	buffer[REFLECTCOLOR] = cast_ray(scene, hit.point, buffer[REFLECTDIR], depth);
	return ((t_vec)handle_impact(scene, buffer, dir, hit));
}
