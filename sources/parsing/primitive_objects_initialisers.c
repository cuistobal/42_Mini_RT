#include "minirt.h"

//
void	*sphere_initialiser(char *input, void *data)
{
	t_sphere	*this;

	this = (t_sphere)data;	
    if (!extract_vector_from_string(&this->center, str, convert, NULL))
        return (NULL);
    if (!extract_float_from_string(&this->color, str, convert, NULL))
        return (NULL);
    if (!get_colors(object, str))
		return (NULL);
	return (endOfScenario(*str, this));
}

//
void	*plane_initialiser(char *input, void *data)
{
	t_plane	*this;
	float	limits[2];

	this = (t_plane)data;
	limits[0] = VECLIMIN;
	limits[1] = VECLIMAX;
    if (!extract_vector_from_string(object, str, convert, NULL))
		return (NULL);
    if (!extract_vector_from_string(object, str, convert, limits))
		return (NULL);
    if (!get_colors(&this->color, str))
		return (NULL);
	return (endOfScenario(*str, this));
}

//
void	*cylinder_initialiser(char *input, void *data)
{
	t_cylinder	*this;
	float		lim[3];

	lim[0] = VECLIMIN;
	lim[1] = VECLIMAX;
	this = (t_cylinder)data;
    if (!extract_vector_from_string(&this->center, str, convert, NULL))
        return (NULL); 
    if (!extract_vector_from_string(&this->normalized_axis, str, convert, lim))
        return (NULL);
    if (!extract_float_from_string(&this->diameter, str, convert, NULL)	
		return (NULL);
	if (!extract_float_from_string(&this->height, str, convert, NULL))
		return (NULL);
	else if (!get_colors(&this->color, str))
		return (NULL);
	return (endOfScenario(*str, this));
}
