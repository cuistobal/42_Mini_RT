#include "minirt.h"

//
void	*camera_initialiser(char **str, void *data, t_prim *pdata)
{
	t_camera	*this;
	float		limits[2];

	this = (t_camera *)data;
	if (!extract_vector_from_string(&this->view_point, str, convert, NULL))
        return (NULL);
	if (!extract_vector_from_string(&this->normalized_orientation, str, convert, NULL))
        return (NULL);
	limits[0] = RATIOLIMIN;
	limits[1] = FOVCOEF;
    if (!extract_float_from_string(&this->fov, str, convert, limits))
        return (NULL);
	else
		set_standard_material(&pdata->material);
	pdata->normalized_axis = set_vec_value(0, 0, 0);
	pdata->boundaries = set_aabb_value(set_vec_value(0, 0, 0), set_vec_value(0, 0, 0));
	pdata->color = 0xFFFFFF;
	return (end_of_scenario(*str, &this));
}

//
void	*ambient_initialiser(char **str, void *data, t_prim *pdata)
{
	t_ambient	*this;
	float		limits[2];

	limits[0] = RATIOLIMIN;
	limits[1] = RATIOLIMAX;
	this = (t_ambient *)data;
    if (!extract_float_from_string(&this->light_ratio, str, convert, limits))
        return (NULL);
    if (!get_colors(&pdata->color, str))
		return (NULL);
	set_standard_material(&pdata->material);
	pdata->normalized_axis = set_vec_value(0, 0, 0);
	pdata->boundaries = set_aabb_value(set_vec_value(0, 0, 0), set_vec_value(0, 0, 0));
	return (end_of_scenario(*str, &this));
}

//
void	*light_initialiser(char **str, void *data, t_prim *pdata)
{
	t_light	*this;
	float	limits[2];

	this = (t_light *)data;
	if (!extract_vector_from_string(&this->light_position, str, convert, NULL))
    	return (NULL);
	limits[0] = RATIOLIMIN;
	limits[1] = RATIOLIMAX;
    if (!extract_float_from_string(&this->brightness, str, convert, limits))
		return (NULL);
    if (!get_colors(&pdata->color, str))
		return (NULL);
	else
		set_standard_material(&pdata->material);
	pdata->normalized_axis = set_vec_value(0, 0, 0);
	pdata->boundaries = set_aabb_value(set_vec_value(0, 0, 0), set_vec_value(0, 0, 0));
	return (end_of_scenario(*str, &this));
}
