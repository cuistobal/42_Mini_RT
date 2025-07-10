#include "minirt.h"

//Yes indeed -> return value is >= 0.f
static float	get_plane_intersection(t_object *obj, t_vec orig, t_vec dir)
{
    float	d;
    t_vec	diff;
	t_vec	center;
	t_vec	normalized_axis;

	center = obj->pdata.center;
	normalized_axis = obj->pdata.normalized_axis;
    d = vec_dot(normalized_axis, dir);
    if (fabsf(d) < 1e-6)
        return (-1.0f);
    diff = vec_sub(center, orig);
    return (vec_dot(diff, normalized_axis) / d);
}

//Do we have a plae intersection in that direction ?
bool	plane_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
    float	intersection;

    intersection = get_plane_intersection(obj, orig, dir);
	if (intersection < 0)
        return (false);
    hit->hit = true;
    hit->distance = intersection;
//	hit->color = ;
    hit->point = vec_add(orig, vec_scale(dir, intersection));
    hit->normal = obj->pdata.normalized_axis;
	hit->material = obj->pdata.material;
    return (true);
}
