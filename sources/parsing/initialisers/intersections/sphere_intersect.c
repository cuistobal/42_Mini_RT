#include "minirt.h"

//Finds out if the ray has at least 1 intersection with the sphere. If so,
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
	d2 = vec_dot(L, L) - sqrt(vec_dot(L, dir));
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
    return true;
}
