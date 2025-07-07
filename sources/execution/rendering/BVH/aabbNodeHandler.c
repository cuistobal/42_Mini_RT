#include "minirt.h"

//
float	get_aabb_surface_area(t_aabb boundaries)
{
	float	surface;
	float	dimensions[3];

	surface = 0;
	dimensions[X] = boundaries.max_vec.x - boundaries.min_vec.x;
	dimensions[Y] = boundaries.max_vec.y - boundaries.min_vec.y;
	dimensions[Z] = boundaries.max_vec.z - boundaries.min_vec.z;
	surface += dimensions[X] * dimensions[Y]; 
	surface += dimensions[Y] * dimensions[Z]; 
	surface += dimensions[Z] * dimensions[X]; 
	return (surface * SURFACE_COEFFICIENT);	
}

//
void	turn_vectors_to_aabb(t_aabb *dest, t_vec v1, t_vec v2)
{
    dest->min_vec.x = fmin(v1.x, v2.x);
    dest->min_vec.y = fmin(v1.y, v2.y);
    dest->min_vec.z = fmin(v1.z, v2.z);
    dest->max_vec.x = fmax(v1.x, v2.x);
    dest->max_vec.y = fmin(v1.y, v2.y);
    dest->max_vec.z = fmin(v1.z, v2.z);
}

//
t_aabb	combine_aabb_nodes(t_aabb v1, t_aabb v2)
{
	t_vec	min;
	t_vec	max;

	min.x = fmin(v1.min_vec.x, v2.min_vec.x);
	min.y = fmin(v1.min_vec.y, v2.min_vec.y);
	min.z = fmin(v1.min_vec.z, v2.min_vec.z);
	max.x = fmax(v1.max_vec.x, v2.max_vec.x);
	max.y = fmax(v1.max_vec.y, v2.max_vec.y);
	max.z = fmax(v1.max_vec.z, v2.max_vec.z);
	return ((t_aabb){(t_vec)min, (t_vec)max}); 
}

//
t_aabb	set_aabb_value(t_vec min_vec, t_vec max_vec)
{
	return ((t_aabb){min_vec, max_vec});
}

//
t_aabb	create_empty_aabb_node(void)
{
	t_vec	min;
	t_vec	max;

	min = set_vec_value(-INFINITY, -INFINITY, -INFINITY);
	max = set_vec_value(INFINITY, INFINITY, INFINITY);
	return ((t_aabb){min, max});
}
