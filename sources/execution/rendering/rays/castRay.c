#include "minirt.h"

void	init_material_values(t_material *material)
{
	material = NULL;
}

//
void	init_hit_values(t_hit *hit)
{
	hit->hit = false;
	hit->distance = 1e30;
	hit->point = set_vec_value(0, 0, 0);
	hit->normal = set_vec_value(0, 0, 0);
	init_material_values(&hit->material);
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

/*	We need multithreading here :)	
void	handle_multiple_lights(t_scene *scene, t_hit *hit, float *dli, float *sli)
{
	t_object	*lights;
    t_vec		light_dir;
    t_hit		shadow_hit;

	lights = scene->lights;
	while (lights)
	{
		init_hit_values(&shadow_hit);
        light_dir = vec_normalized(vec_sub(scene->lights[i], hit.point));
        if (scene_intersect(scene, hit.point, light_dir, &shadow_hit) &&
            vec_norm(vec_sub(shadow_hit.point, hit.point)) < vec_norm(vec_sub(scene->lights[i], hit.point)))
            continue;
        *dli += fmaxf(0.0f, vec_dot(light_dir, hit.normal));
        *sli += powf(fmaxf(0.0f, -vec_dot(reflect(vec_negate(light_dir), hit.normal), dir)), hit.material.specular_exponent);
		lights = lights->next;
    }
}


//dli -> diffuse light intensity
//sli -> specular light intensity
//
static void	handle_impact(t_scene *scene, t_vec buffer[], t_vec dir, t_hit hit)
{
	float	dli;
	float	sli;
	t_hit	shadow;
	t_vec	buff[4];
	t_vec	light_dir;

	dli = 0;
	sli = 0;
	buff[REFLECTDIR] = vec_normalized(reflect(dir, hit.normal));

	if (material) //Can be removed once we're sure all objects have a material
		buff[REFRACTDIR] = vec_normalized(refract(dir, hit.normal, \
					hit.material.refractive_index, 1.0f)) ;

	buff[REFLECTCOLOR] = cast_ray(scene, hit.point, buff[REFLECTDIR], depth + 1);
//	buff[REFRACTCOLOR] = Cast_ray(scene, hit.point,	buff[REFRACTDIR], depth + 1);

	//bonus function -> handle multiple lights

	if (hit.material)
	{
    	buffer[DIFFUSE] = vec_scale(hit.material.diffuse_color, \
				dli * hit.material.albedo[0]);
    	buffer[SPECULAR] = vec_scale(set_vec_values(1.0, 1.0, 1.0), \
				sli * hit.material.albedo[1]);
    	buffer[REFLECTION] = vec_scale(reflect_color, hit.material.albedo[2]);
    	buffer[REFRACTION] = vec_scale(refract_color, hit.material.albedo[3]);
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
        if (current->methods->intersect(current, orig, dir, &hit) && hit.distance < min)
		{
            min = hit.distance;
            *closest_hit = hit;
            found = true;
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

	if (depth > MAXRAYDEPTH)
		return ((t_vec)set_vec_value(0.2, 0.7, 0.8));
	init_hit_values(&hit);
	if (!scene_intersect(scene, orig, dir, &hit))
		return ((t_vec)set_vec_value(0.2, 0.7, 0.8));

//Bonus version tbh
//	handle_impact(scene, buffer);
//
	return (cast_ray_return(buffer));
}
