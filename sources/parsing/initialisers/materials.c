#include "minirt.h"

//Will be handled in bonuses only
void	set_standard_material(t_material *m)
{
	m->set = false;
	ft_bzero(m->albedo, sizeof(float) * 4);
	m->refractive_index = 0;
	m->specular_exponent = 0;
	m->diffuse_color = set_vec_value(0, 0, 0);
}
