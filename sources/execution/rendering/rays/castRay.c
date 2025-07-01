#include "minirt.h"

//
void	init_hit_values(t_hit *hit)
{
	hit->hit = false;
	hit->distance = 1e30;
	set_empty_vec(hit->point);
	set_empty_vec(hit->normal);
	material = NULL;
}

//Returns the sum of all the vectors contained in our computed buffer. Hence,
//we get a vector that represents the light status of our ray;
static inline t_vec	cast_ray_return(t_vec buffer[])
{
	t_vec	sumof1;
	t_vec	sumof2;

	sumof1 = vecAdd(buffer[DIFFUSE], buffer[SPECULAR]);
	sumof2 = vecAdd(buffer[REFLECTION], buffer[REFRACTION]);
	return (vecAdd(sumof1, sumof2);
}

/*	We need multithreading here :)	
void handle_multiple_lights()
{
    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (size_t i = 0; i < scene->light_count; i++) {
        t_vec light_dir = vec3_normalized(vec3_sub(scene->lights[i], hit.point));
        t_hit shadow_hit = {0};
        if (scene_intersect(scene, hit.point, light_dir, &shadow_hit) &&
            vec3_norm(vec3_sub(shadow_hit.point, hit.point)) < vec3_norm(vec3_sub(scene->lights[i], hit.point)))
            continue;
        diffuse_light_intensity += fmaxf(0.0f, vec3_dot(light_dir, hit.normal));
        specular_light_intensity += powf(fmaxf(0.0f, -vec3_dot(reflect(vec3_negate(light_dir), hit.normal), dir)), hit.material.specular_exponent);
    }

    t_vec diffuse = vec3_scale(hit.material.diffuse_color, diffuse_light_intensity * hit.material.albedo[0]);
    t_vec specular = vec3_scale((t_vec){1.0, 1.0, 1.0}, specular_light_intensity * hit.material.albedo[1]);
    t_vec reflection = vec3_scale(reflect_color, hit.material.albedo[2]);
    t_vec refraction = vec3_scale(refract_color, hit.material.albedo[3]);

    return vec3_add(vec3_add(diffuse, specular), vec3_add(reflection, refraction));
}
*/

//dli -> diffuse light intensity
//sli -> specular light intensity
//
void	handle_impact(t_scene *scene, t_vec buffer[])
{
	float	dli;
	float	sli;
	t_hit	shadow;
	t_vec	buff[4];
	t_vec	light_dir;

	dli = 0;
	sli = 0;
	buff[REFLECTDIR] = vecNormalized(reflect(dir, hit.normal));
	if (material) //Can be removed once we're sure all objects have a material
		buff[REFRACTDIR] = vecNormalized(refract(dir, hit.normal, \
					hit.material.refractive_index, 1.0f)) ;
	buff[REFLECTCOLOR] = cast_ray(scene, hit.point, buff[REFLECTDIR], depth + 1);
	buff[REFRACTCOLOR] = Cast_ray(scene, hit.point,	buff[REFRACTDIR], depth + 1);

	//bonus function -> handle multiple lights

	if (hit.material)
	{
    	buffer[DIFFUS] = vec_scale(hit.material.diffuse_color, \
				dli * hit.material.albedo[0]);
    	buffer[SPECULAR] = vec3_scale({1.0, 1.0, 1.0}, \
				sli * hit.material.albedo[1]);
    	buffer[REFLECTION] = vec3_scale(reflect_color, hit.material.albedo[2]);
    	buffer[REFRACTION] = vec3_scale(refract_color, hit.material.albedo[3]);
	}
    return (cast_ray_return(buffer));
}

//
bool	scene_intersect(t_scene *scene, t_vec orig, t_vec dir, \
		t_hit *closestHit)
{
    bool		found;
    float 		minDist;
    t_hit		hitpoint;
	t_object	*current;

	found = false;
    minDist = 1e30;
	current = scene->objects;
	hitpoint  = {0}; //APpend real values and see if norms allows
	while (current)
	{
		hitpoint  = {0}; //APpend real values and see if norms allows
        if (obj->intersect(obj, orig, dir, &hit) && hit.distance < minDist)
		{
            minDist = hit.distance;
            *closest_hit = hit;
            found = true;
        }
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
		return (setVecValues(0.2, 0.7, 0.8);
	init_hit_values(&hit);
	if (!scene_intersect(scene, orig, dir, &hit))
		return (setVecValues(0.2, 0.7, 0.8);

//Bonus version tbh
	handle_impact(scene, buffer);
//
	return (cast_ray_return(buffer));
}
