#include "minirt.h"

static bool	get_plane_intersection(t_plane *p, t_vec orig, t_vec dir, float *t)
{
    float	d;
    t_vec	diff;

    d = vec_dot(p->normalized_axis, dir);
    if (fabsf(d) < 1e-6)
        return (false);
    diff = vec_sub(p->center, orig);
    *t = vec_dot(diff, p->normalized_axis) / d;
    return (*t >= 0);
}

static void	fill_plane_hit(t_plane *p, t_vec orig, t_vec dir, float t, t_hit *hit)
{
    hit->hit = true;
    hit->distance = t;
    hit->point = vec_add(orig, vec_scale(dir, t));
    hit->normal = p->normalized_axis;
}

bool	plane_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
    float	t;
    t_plane	*p;

    p = &obj->u_type.plane;
    if (!get_plane_intersection(p, orig, dir, &t))
        return (false);
    if (hit)
        fill_plane_hit(p, orig, dir, t, hit);
    return (true);
}
