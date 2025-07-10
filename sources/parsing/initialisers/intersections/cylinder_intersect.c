#include "minirt.h"

//
static float	get_cylinder_c(float diameter, t_vec axis, t_vec oc)
{
    float	axis_dot;
    float	diameter_half;

    axis_dot = vec_dot(oc, axis);
    diameter_half = diameter / 2.0f;
    return (vec_dot(oc, oc) - powf(axis_dot, 2) - powf(diameter_half, 2));
}

//We get 2 intersections, If the first 1 has a negativ value (meaning its 
//hidden), we try to return the second value (outer intersection). Won't happen
// in the mandatory part since we don't handle reflection/refraction. Can happen
//in bonus when we represent the flection of an objet lying behind the camera
//(typical mirror scenario)
static bool	solve_cylinder_quadratic(float a, float b, float c, float *t)
{
    float	sqrt_disc;
    float	discriminant;

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (false);
    sqrt_disc = sqrtf(discriminant);
    *t = (-b - sqrt_disc) / (2 * a);
	if (*t < 0)	
    	*t = (-b + sqrt_disc) / (2 * a);
    return (*t >= 0);
}

static bool	get_cylinder_intersection(t_object *cyl, t_vec orig, t_vec dir, float *t)
{
    float	a;
    float	b;
    float	cc;
    t_vec	oc;
	t_vec	na;

	na = cyl->pdata.normalized_axis;
    oc = vec_sub(orig, cyl->pdata.center);
    a = vec_dot(dir, dir) - powf(vec_dot(dir, na), 2);
    b = 2 * (vec_dot(dir, oc) - vec_dot(dir, na) * vec_dot(oc, na));
    cc = get_cylinder_c(cyl->u_type.cylinder.diameter, na, oc);
    return (solve_cylinder_quadratic(a, b, cc, t));
}

static t_vec	compute_cp(t_vec orig, t_vec dir, t_vec center, float t)
{
	t_vec	scaling;
	t_vec	addition;
	t_vec	substracted;

	scaling = vec_scale(dir, t);
	addition = vec_add(orig, scaling);
	substracted = vec_sub(addition, center);
	return ((t_vec){substracted.x, substracted.y, substracted.z});
}

bool	cylinder_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit)
{
    t_vec	cp;
    t_vec	axis;
    float	distance;
		
	axis = obj->pdata.normalized_axis;
    if (!get_cylinder_intersection(obj, orig, dir, &distance))
        return (false);
    cp = compute_cp(orig, dir, obj->pdata.center, distance);
    hit->hit = true;
    hit->distance = distance;
    hit->point = vec_add(orig, vec_scale(dir, distance));
    hit->normal = vec_normalized(vec_sub(cp, vec_scale(axis, vec_dot(cp, axis))));
	hit->material = obj->pdata.material;
	//print_vec("cylinder intersection", hit->point);
    return (true);
}
