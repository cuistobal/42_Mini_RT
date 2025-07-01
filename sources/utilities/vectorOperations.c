#include "minirt.h"

//tb modified
inline t_vec	vecAdd(t_vec a, t_vec b)
{
	return (t_vec){a.x + b.x, a.y + b.y, a.z + b.z};
}

//tb modified
inline t_vec	vecSub(t_vec a, t_vec b)
{
	return (t_vec){a.x - b.x, a.y - b.y, a.z - b.z};
}

//isoke ?
inline void	vecScale(t_vec *vector, float s)
{
	vector->x *= s;
	vector->y *= s;
	vector->z *= s;
}

//
inline float	vecDot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
inline float	vecNorm(t_vec v)
{
	return (sqrtf(vecDot(v, v)));
}

inline void	vecNormalized(t_vec *v)
{
/*
	float n;

	n = vecNorm(*v);
	vecScale(v, 1.0f / n);
*/
	vecScale(v, 1.0f / vecNorm(*v));
}

inline void	vecNegate(t_vec *v)
{
	v->x *= -1;
	v->y *= -1;
	v->z *= -1;
}

t_vec vecCross(t_vec a, t_vec b)
{
    return (t_vec){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

inline void	setEmptyVec(t_vec *v)
{
	v->x = 0;
	v->y = 0;
	v->z = 0;
}
