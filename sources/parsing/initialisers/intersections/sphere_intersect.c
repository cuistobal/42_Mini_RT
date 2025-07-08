#include "minirt.h"

/*
//
static float get_squared_radius(const t_object *obj)
{
    return (powf(obj->u_type.sphere.diameter / 2.0f, 2));
}

//
static float sphere_get_tca(t_vec L, t_vec dir)
{
    return vec_dot(L, dir);
}

//
static float sphere_get_d2(t_vec L, float tca)
{
    return vec_dot(L, L) - tca * tca;
}

//
static float sphere_get_t(float tca, float r2, float d2)
{
	float	t;
    float	thc;

	thc = sqrtf(r2 - d2);
	t = tca - thc;
	if (t > 0)
		return (t);
	return (tca + thc);
}

//
bool sphere_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
    t_vec 	L;
	float	t;
    float 	d2;
    float 	r2;
    float 	tca;

    L = vec_sub(obj->pdata.center, orig);
    tca = sphere_get_tca(L, dir);
    d2 = sphere_get_d2(L, tca);
    r2 = get_squared_radius(obj);
	t = sphere_get_t(tca, r2, d2);
    if (d2 <= r2 && t >= 0)
	{
		hit->hit = true;
		hit->distance = t;
    	hit->point = vec_add(orig, vec_scale(dir, t));
    	hit->normal = vec_normalized(vec_sub(hit->point, obj->pdata.center));
    	hit->material = obj->pdata.material;
		print_vec("sphere intersection", hit->point);
	}
	return (hit->hit);
}
*/
bool sphere_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
	t_vec	oc;

    oc = vec_sub(orig, obj->pdata.center);
    float (a) = vec_dot(dir, dir);
    float (b) = 2.0f * vec_dot(oc, dir);
    float (c) = vec_dot(oc, oc) - powf(obj->u_type.sphere.diameter / 2.0f, 2.0f);
    float (discriminant) = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;
    float (sqrt_disc) = sqrtf(discriminant);
    float (t1) = (-b - sqrt_disc) / (2.0f * a);
    float (t2) = (-b + sqrt_disc) / (2.0f * a);
    float (t) = (t1 > 0.001f) ? t1 : (t2 > 0.001f ? t2 : -1.0f);
    if (t < 0)
        return false;
    hit->distance = t;
    hit->point = vec_add(orig, vec_scale(dir, t));
    hit->normal = vec_normalized(vec_sub(hit->point, obj->pdata.center));
    hit->hit = true;
    return true;
}
