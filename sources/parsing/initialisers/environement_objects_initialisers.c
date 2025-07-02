#include "minirt.h"


//
void	*camera_initialiser(char **str, void *data, t_prim *pdata)
{
	t_camera	*this;
	char		*temp;
	float		limits[2];

	pdata = NULL;
	this = (t_camera *)data;
    temp = ft_strtok_r(*str, WHITESPACES, str);
	if (!extract_vector_from_string(&this->view_point, str, convert, NULL))
        return (NULL);
	if (!extract_vector_from_string(&this->normalized_orientation, str, convert, NULL))
        return (NULL);
	limits[0] = RATIOLIMIN;
	limits[1] = FOVCOEF;
    if (!extract_float_from_string(&this->fov, &temp, convert, limits))
        return (NULL);
	else
		return (endOfScenario(*str, this));
}

//
void	*ambient_lighting_initialiser(char **str, void *data, t_prim *pdata)
{

	char		*temp;
	t_ambient	*this;
	float		limits[2];

	pdata = NULL;
	limits[0] = RATIOLIMIN;
	limits[1] = RATIOLIMAX;
	this = (t_ambient *)data;
    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!extract_float_from_string(&this->light_ratio, &temp, convert, limits))
        return (NULL);
    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!get_colors(&this->color, &temp))
		return (NULL);
	return (endOfScenario(*str, this));
}

//
void	*light_initialiser(char **str, void *data, t_prim *pdata)
{
	t_light	*this;
	float	limits[2];

	pdata = NULL;
	this = (t_light *)data;
	if (!extract_vector_from_string(&this->light_position, str, convert, NULL))
    	return (NULL);
	limits[0] = RATIOLIMIN;
	limits[1] = RATIOLIMAX;
    if (!extract_float_from_string(&this->brightness, str, convert, limits))
		return (NULL);
    if (!get_colors(&this->color, str))
		return (NULL);
	return (endOfScenario(*str, this));
}
