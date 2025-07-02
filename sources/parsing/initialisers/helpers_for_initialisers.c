#include "minirt.h"

void	*endOfScenario(char *str, void *object)
{
	while (*str && isspace(*str))
		str++;
	if (!*str)
		return (object);
	return (NULL);
}

//
inline static bool	check_limits(bool status, float value, float limin, float limax)
{
	return (status && value >= limin && value <= limax);
}

//
bool	extract_float_from_string(float *value, char **str, \
		bool (*convert)(float *dst, char *src), float limits[])
{ 
    char    *temp;
    bool    status;

    status = true;
    temp = ft_strtok_r(*str, WHITESPACES, str);
    status = convert(value, temp);
	if (limits)
    	return (check_limits(status, *value, limits[0], limits[1]));
	return (status);
}

//
bool	extract_vector_from_string(t_vec *v, char **str, \
		bool (*convert)(float *dst, char *src), float limits[])
{
	int		i;
	char	*temp;
	char	*param;
	bool	status;
	float	arr[3];

	i = 0;
	temp = *str;
	status = true;
    while (status && i++ < 3)
    {
        param = ft_strtok_r(temp, ",", &temp);
        status = convert(&arr[i], param);
        if (limits)
            status = check_limits(status, arr[i], limits[0], limits[1]);
    }
	if (status && i == 4)
		return (*v = set_vec_values(arr[0], arr[1], arr[2]), true);
	return (false);
}

//
void	*(*get_initializer(int type))(char **, void *, t_prim *pdata)
{
    if (type == EALIGHT)
		return (ambient_lighting_initialiser);
	else if (type == ECAMERA)
        return (camera_initialiser);
	else if (type == ELIGHT)
		return (light_initialiser);
	else if (type == ESPHERE)
		return (sphere_initialiser);
	else if (type == EPLANE)
		return (plane_initialiser);
	else if (type == ECYLINDER)
		return (cylinder_initialiser);
    return (NULL);
}
