#include "minirt.h"

static float	clamp(float x)
{
	if (x < 0.0f)
		return (0.0f);
	if (x > 1.0f)
		return (1.0f);
	return (x);
}

static float	apply_gamma(float x)
{
	return (powf(x, 1.0f / 2.2f));  // Standard gamma value of 2.2
}

t_vec	clamp_color(t_vec color)
{
	return ((t_vec){
		clamp(color.x),
		clamp(color.y),
		clamp(color.z)
	});
}

t_vec	apply_gamma_correction(t_vec color)
{
	return ((t_vec){
		apply_gamma(color.x),
		apply_gamma(color.y),
		apply_gamma(color.z)
	});
}

int	vec_to_color(t_vec v)
{
	t_vec	corrected;
	int		r;
	int		g;
	int		b;

	corrected = apply_gamma_correction(clamp_color(v));
	r = (int)(255.0f * corrected.x);
	g = (int)(255.0f * corrected.y);
	b = (int)(255.0f * corrected.z);
	return ((r << 16) | (g << 8) | b);
}
