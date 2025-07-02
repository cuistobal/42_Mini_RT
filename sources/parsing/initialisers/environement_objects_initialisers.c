#include "minirt.h"

//
inline void *endOfScenario(char *str, void *object)
{
	while (*str && isspace(*str))
		str++;
	if (!*str)
		return (object);
	return (NULL);
}

//
void	*camera_initialiser(char **str, void *data)
{
	t_camera	*this;
	float		limits[2];

	this = (t_camera *)data;
	if (!getCoordinates(&this->view_point, str, convert, NULL))
        return (NULL);
	if (!getCoordinates(&this->normalized_orientation, str, convert, NULL))
        return (NULL);
	limits[0] = RATIOLIMIN;
	limits[1] = FOVCOEF;
    if (!extract_float_from_string(&this->fov, &temp, convert, limits))
        return (NULL);
	return (endOfScenario(*str, this));
}

//
void	*ambient_lighting_initialiser(char **str, void *data){

	char		*temp;
	t_ambient	*this;
	float		limits[2];

	limits[0] = RATIOLIMIN:
	limits[1] = RATIOLIMAX;
	this = (t_ambient *)data;
    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!extract_float_from_string(&this->light_ratio, &temp, convert, limits))
        return (NULL);
    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!get_colors(&temp, this->color))
		return (NULL);
	return (endOfScenario(*str, this));
}

//
void	*light_initialiser(char **str, void *data)
{
	t_light	*this;
	float	limits[2];

	this = (t_light *)data;
	if (!extract_float_from_string(&this->light_position, str, convert, NULL))
    	return (NULL);
	limits[0] = RATIOLIMIN;
	limits[1] = RATIOLIMAX;
    if (!extract_float_from_string(object->ratio[0], str, convert, limits))
		return (NULL);
    if (!get_color(&object->brightness, str))
		return (NULL);
	return (endOfScenario(*str, this));
}
