#include "minirt.h"

void	*end_of_scenario(char *str, void *object)
{
	while (*str && isspace(*str))
		str++;
	if (!*str || !strcmp(str, ""))
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
	status = true;
    temp = ft_strtok_r(*str, WHITESPACES, str);
    while (status && i++ < 3)
    {
        param = ft_strtok_r(temp, ",", &temp);
        status = convert(&arr[i], param);
        if (limits)
            status = check_limits(status, arr[i], limits[0], limits[1]);
    }
	if (status && i == 4)
		return (*v = set_vec_value(arr[0], arr[1], arr[2]), true);
	return (false);
}

void	*call_object_initializer(char type, char **line, t_object *obj)
{
    if (type == ESPHERE)
        return (obj->methods.initializer(line, &obj->u_type.sphere, &obj->pdata));
    else if (type == EPLANE)
        return (obj->methods.initializer(line, &obj->u_type.plane, &obj->pdata));
    else if (type == ECYLINDER)
        return (obj->methods.initializer(line, &obj->u_type.cylinder, &obj->pdata));
    else if (type == ECAMERA)
        return (obj->methods.initializer(line, &obj->u_type.camera, &obj->pdata));
    else if (type == ELIGHT)
        return (obj->methods.initializer(line, &obj->u_type.light, &obj->pdata));
    else if (type == EALIGHT)
        return (obj->methods.initializer(line, &obj->u_type.ambient, &obj->pdata));
    return (NULL);
}
