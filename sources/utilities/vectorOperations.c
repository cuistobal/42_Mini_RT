#include "minirt.h"


//This function takes two vectors a and b and returns a new vector whose
//components are the sum of the corresponding components of a and b.
inline t_vec	vec_add(t_vec a, t_vec b)
{
	return (t_vec){a.x + b.x, a.y + b.y, a.z + b.z};
}

//This function takes two vectors a and b and returns a new vector whose
//components are the result of subtracting the components of b from the 
//corresponding components of a.
inline t_vec	vec_sub(t_vec a, t_vec b)
{
	return (t_vec){a.x - b.x, a.y - b.y, a.z - b.z};
}

//This function takes a vector v and a scalar s, and returns a new vector whose
//components are the components of v multiplied by the scalar s.
inline t_vec	vec_scale(t_vec vector, float s)
{
	return ((t_vec){vector.x * s, vector.y * s, vector.z * s});
}

//This function takes two vectors a and b and returns a scalar value which is 
//the sum of the products of the corresponding components of a and b
inline float	vec_dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//This function takes a vector v and returns its magnitude (or length). It 
//calculates the square root of the dot product of v with itself.
inline float	vec_norm(t_vec v)
{
	return (sqrtf(vec_dot(v, v)));
}
