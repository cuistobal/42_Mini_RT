#include "minirt.h"

//
t_aabb	sphere_bounds(t_object *obj)
{
    t_vec		axis1;
	t_vec		axis2;
    float		radius;

	radius = obj->u_type.sphere.diameter / 2.0f;
    axis1.x = obj->pdata.center.x - radius;
    axis1.y = obj->pdata.center.y - radius;
    axis1.z = obj->pdata.center.z - radius;
    axis2.x = obj->pdata.center.x + radius;
    axis2.y = obj->pdata.center.y + radius;
    axis2.z = obj->pdata.center.z + radius;
	turn_vectors_to_aabb(&obj->pdata.boundaries, axis1, axis2);	
	return (obj->pdata.boundaries); 
}

//
t_aabb	no_bounds(t_object *obj)
{
	t_vec	axis1;
	t_vec	axis2;

	axis1 = set_vec_value(0, 0, 0);
	axis2 = set_vec_value(0, 0, 0);
	turn_vectors_to_aabb(&obj->pdata.boundaries, axis1, axis2);	
	return (obj->pdata.boundaries); 
}

//
t_aabb	cylinder_bounds(t_object *obj)
{
    t_vec		axis1;
	t_vec		axis2;
    float		radius;
	float		heightr;

	heightr = obj->u_type.cylinder.height / 2.0f;
	radius = obj->u_type.cylinder.diameter / 2.0f;
    axis1.x = obj->pdata.center.x + obj->pdata.normalized_axis.x * heightr;
    axis1.y = obj->pdata.center.y + obj->pdata.normalized_axis.y * heightr;
    axis1.z = obj->pdata.center.z + obj->pdata.normalized_axis.z * heightr;
    axis2.x = obj->pdata.center.x - obj->pdata.normalized_axis.x * heightr;
    axis2.y = obj->pdata.center.y - obj->pdata.normalized_axis.y * heightr;
    axis2.z = obj->pdata.center.z - obj->pdata.normalized_axis.z * heightr;
	turn_vectors_to_aabb(&obj->pdata.boundaries, axis1, axis2);
	return (obj->pdata.boundaries); 
}
