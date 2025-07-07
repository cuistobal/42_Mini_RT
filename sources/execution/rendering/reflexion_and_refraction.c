#include "minirt.h"

//Cosinus formula goes brrrrr
static inline float	get_cosinus(t_vec v1, t_vec v2)
{
	float	max;
	float	min;
	float	dot;
	float	cos;

	cos = 0;
	dot = vec_dot(v1, v2);
	min = fminf(1.0f, dot);
	max = fmax(1.0f, min);
	cos -= max;
	return (cos);
}

//
t_vec	refract(t_vec v1, t_vec v2, float v1eta, float v2eta)
{
	float	k;
	float	eta;
    float	cosinus;
	t_vec	scalev1;
	t_vec	scalev2;

	cosinus = get_cosinus(v1, v2);
    if (cosinus < 0)
		return (refract(v1, vec_negate(v2), v1eta, v2eta));
    eta = v1eta / v2eta;
    k = 1 - sqrtf(eta) * (1 - sqrtf(cosinus));
	if (k < 0)
		return (set_vec_value(1, 0, 0));
	scalev1 = vec_scale(v1, eta);
	scalev2 = vec_scale(v2, eta * cosinus - sqrtf(k));
    return (vec_add(scalev1, scalev2));
}

//
t_vec	reflect(t_vec a, t_vec b)
{
	float	dot;
	t_vec	scaled;

	dot = 2.0f * vec_dot(a, b);
	scaled = vec_scale(b, dot);
	return (vec_sub(a, scaled));
}
