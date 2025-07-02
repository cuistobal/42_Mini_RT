#include "minirt.h"

//This function takes a vector v and returns a new vector that is the 
//normalized version of v. Normalization means converting the vector to have a 
//magnitude of 1 while maintaining its direction. This is done by dividing each
//component of the vector by its magnitude.
inline t_vec	vec_normalized(t_vec v)
{
	return (vec_scale(v, 1.0f / vec_norm(v)));
}

//This function takes a vector v and returns a new vector whose components are 
//the negation of the components of v. This operation is known as vector 
//negation.
inline t_vec	vec_negate(t_vec v)
{
	return ((t_vec){-v.x, -v.y, -v.z});
}

//This function takes two vectors a and b and returns a new vector that is the 
//cross product of a and b. The cross product is a vector that is perpendicular
//to both a and b, and its magnitude is equal to the area of the parallelogram 
//that the vectors span.
inline t_vec	vec_cross(t_vec a, t_vec b)
{
    return ((t_vec){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, \
			a.x * b.y - a.y * b.x});
}

//Returns a vector containing the values passed as parameters
inline t_vec	set_vec_value(float x, float y, float z)
{
	return ((t_vec){x, y, z});
}
