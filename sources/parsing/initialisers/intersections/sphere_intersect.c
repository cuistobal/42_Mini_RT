#include "minirt.h"

/* //Finds out if the ray has at least 1 intersection with the sphere. If so,
//appends hit's value and return true. We only care about the leading impact
//because fuck the norminette :) and we can always reverse t to get it for
//reflection rays.
bool	sphere_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
    t_vec		L;
	float		t;
	float		d2;
	float		r2;
	float		thc;

    L = vec_sub(obj->pdata.center, orig);
	r2 = powf(obj->u_type.sphere.diameter / 2, 2);
	d2 = vec_dot(L, L) - sqrtf(vec_dot(L, dir));
	printf("vd(L)	->	%f\n", vec_dot(L, L));
	printf("vd(Ld)	->	%f\n", vec_dot(L, dir));
	printf("sq root	->	%f\n", sqrtf(vec_dot(L, dir)));
	if (d2 > r2)
		return (false);
	thc = sqrtf(r2 - d2);
	t = vec_dot(L, dir) - thc;
	if (t < 0)
		return (false);
    hit->hit = true;
    hit->distance = t;
    hit->point = vec_add(orig, vec_scale(dir, t));
    hit->normal = vec_normalized(vec_sub(hit->point, obj->pdata.center));
    hit->material = obj->pdata.material;
//	print_vec("normalized sphere intersection", hit->normal);
//	print_vec("point sphere intersection", hit->point);
    return true;
}
	printf("vd(L)	->	%f\n", vec_dot(L, L));
	printf("vd(Ld)	->	%f\n", vec_dot(L, dir));
	printf("sq root	->	%f\n")
 */

#include "minirt.h"

static float get_squared_radius(const t_object *obj)
{
    return (powf(obj->u_type.sphere.diameter / 2.0f, 2));
}

static float sphere_get_tca(t_vec L, t_vec dir)
{
    return vec_dot(L, dir);
}

static float sphere_get_d2(t_vec L, float tca)
{
    return vec_dot(L, L) - tca * tca;
}

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
	}
	return (hit->hit);
}
