#include "minirt.h"

//
t_aabb	sphere_bounds(t_object *obj)
{
    t_vec		axis1;
	t_vec		axis2;
    float		radius;
	t_sphere	sphere;

	sphere = *(t_sphere *)obj->data;
	radius = sphere.diameter / 2.0f;
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
	t_cylinder	cylinder;

	cylinder = *(t_cylinder *)obj->data;
	radius = cylinder.diameter / 2.0f;
    axis1.x = obj->pdata.center.x + obj->pdata.normalized_axis.x * cylinder.height / 2.0f;
    axis1.y = obj->pdata.center.y + obj->pdata.normalized_axis.y * cylinder.height / 2.0f;
    axis1.z = obj->pdata.center.z + obj->pdata.normalized_axis.z * cylinder.height / 2.0f;
    axis2.x = obj->pdata.center.x - obj->pdata.normalized_axis.x * cylinder.height / 2.0f;
    axis2.y = obj->pdata.center.y - obj->pdata.normalized_axis.y * cylinder.height / 2.0f;
    axis2.z = obj->pdata.center.z - obj->pdata.normalized_axis.z * cylinder.height / 2.0f;
	turn_vectors_to_aabb(&obj->pdata.boundaries, axis1, axis2);
	return (obj->pdata.boundaries); 
}
