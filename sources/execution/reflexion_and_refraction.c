#include "minirt.h"

//Cosinus formula goes brrrrr
static inline float	get_cosinus(t_vec v1, t_vec v2)
{
	float	max;
	float	min;
	float	dot;
	float	cos;

	cos = 0;
	dot = vecDot(v1, v2);
	min = fminf(1.0f, dot);
	max = fmax(1.0f, min);
	cos =- max;
	return (cos);
}

//
t_vec	refract(t_vec v1, t_vec v2, float v1eta, float v2eta)
{
	float	k;
	float	eta;
    float	cosinus;
	float	scalev1;
	float	scalev2;

	cosinus = get_cosinus(v1, v2);
    if (cosinus < 0)
		return (refract(I, vecNegate(N), v1eta, v2eta));
    eta = v1eta / v2eta;
    k = 1 - sqrtf(eta) * (1 - sqrtf(cosinus));
	if (k < 0)
		return ({1, 0, 0});
	scalev1 = vecScale(v1, eta);
	scalev2 = vecScale(v2, eta * cosinus - sqrtf(k));
    return vecAdd(scalev1, scalev2);
}

//
t_vec	reflect(t_vec a, t_vec b)
{
	float	dot;
	t_vec	scaled;

	dot = 2.0f * vecDot(a, b);
	scaled = vecScale(b, dot);
	return (vecSub(a, scale));
}
