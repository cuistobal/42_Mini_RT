#include "minirt.h"

static float	get_c_c(t_vec ax, t_vec oc, float d)	
{
    return (vec_dot(oc, oc) - powf(vec_dot(oc, ax), 2) - powf(d, 2) / 4.0f);
}

static bool	get_cylinder_intersection(t_cylinder *c, t_vec orig, t_vec dir, float *t)
{
	float	a;
	float	b;
	float	cc;
    float	disc;
	t_vec	oc;

	oc = vec_sub(orig, c->center);
    a = vec_dot(dir, dir) - powf(vec_dot(dir, c->normalized_axis), 2);
    b = 2 * (vec_dot(dir, oc) - vec_dot(dir, c->normalized_axis) * \
			vec_dot(oc, c->normalized_axis));
	cc = get_c_c(c->normalized_axis, oc, c->diameter);
	disc = b * b - 4 * a *cc; 
    if (disc < 0)
        return (false);
    *t = (-b - sqrtf(disc)) / (2 * a);
	if (*t <= 0)
		*t = (-b + sqrtf(disc)) / (2 * a);
    return (*t >= 0);
}

static void	fill_cylinder_hit(t_cylinder *c, t_vec orig, t_vec dir, float t, t_hit *hit)
{
    t_vec	pt;
    t_vec	ax;
    t_vec	cp;

    pt = vec_add(orig, vec_scale(dir, t));
    ax = c->normalized_axis;
    cp = vec_sub(pt, c->center);
    hit->hit = true;
    hit->distance = t;
    hit->point = pt;
    hit->normal = vec_normalized(vec_sub(cp, vec_scale(ax, vec_dot(cp, ax))));
}

bool	cylinder_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
    t_cylinder	*c;
    float		t;

    c = &obj->u_type.cylinder;
    if (!get_cylinder_intersection(c, orig, dir, &t))
        return (false);
    if (hit)
        fill_cylinder_hit(c, orig, dir, t, hit);
    return (true);
}
