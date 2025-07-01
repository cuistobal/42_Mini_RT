#include "minirt.h"


//This function takes two vectors a and b and returns a new vector whose
//components are the sum of the corresponding components of a and b.
inline t_vec	vecAdd(t_vec a, t_vec b)
{
	return (t_vec){a.x + b.x, a.y + b.y, a.z + b.z};
}

//This function takes two vectors a and b and returns a new vector whose
//components are the result of subtracting the components of b from the 
//corresponding components of a.
inline t_vec	vecSub(t_vec a, t_vec b)
{
	return (t_vec){a.x - b.x, a.y - b.y, a.z - b.z};
}

//This function takes a vector v and a scalar s, and returns a new vector whose
//components are the components of v multiplied by the scalar s.
inline void	vecScale(t_vec *vector, float s)
{
	vector->x *= s;
	vector->y *= s;
	vector->z *= s;
}

//This function takes two vectors a and b and returns a scalar value which is 
//the sum of the products of the corresponding components of a and b
inline float	vecDot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//This function takes a vector v and returns its magnitude (or length). It 
//calculates the square root of the dot product of v with itself.
inline float	vecNorm(t_vec v)
{
	return (sqrtf(vecDot(v, v)));
}
