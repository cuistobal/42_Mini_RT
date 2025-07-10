#include "minirt.h"

static bool convertRGB(char *rgb, long *value)
{
	*value = ft_atoi(rgb);
    return (*value >= 0 && *value <= 255);
}

//static bool purgeWhitespaces()
/*
bool    getColours(t_object *object, char **str)
{
    int     i;
    int     mask;
    char    *param;
	bool	status;

    i = 1;
    mask = 16;
	status = !(!str || !*str);
    while (status && i++ < 4)
    {
        status = isdigit(**str);
        param = ft_strtok_r(*str, ",", str);
       	status = convertRGB(param, &object->colours[i]);
        *object->colours |= object->colours[i] << mask;
        mask = mask - 8;
    }
    return (status);
}
*/

//
bool    get_colors(int *color, char **str)
{
    int     i;
    int     mask;
    char    *param;
	bool	status;
	long	temp[3];

    i = 0;
    mask = 16;
	status = !(!str || !*str);
    while (status && i++ < 3)
    {
        status = isdigit(**str);
        param = ft_strtok_r(*str, ",", str);
		if (!status || !param)
			break ;
       	status = convertRGB(param, &temp[i]);
        *color |= temp[i] << mask;
        mask = mask - 8;
    }
    return (status);
}

bool	convert(float *coordinates, char *str)
{
    bool    error;

    error = true;
    *coordinates = ft_atof(str, &error);
    return (error);
}

/*
t_vec	hit_color(t_hit hit, t_object *obj)
{
	t_vec	color;
	
	color = vec_sub(hit,point, obj->(?))
	return ((t_vec)color);
}

t_rgb	color_disk(t_model *m, t_impact *imp, t_dsk *obj)
{
	t_v3	vec;
	int		f2;
	int		f3;

	(void) m;
	if (!obj->check)
		return (obj->col1);
	vec_sub(vec, imp->pos, obj->pos);
	f2 = roundf(2 * vec_dot(obj->e2, vec) / obj->rad);
	f3 = roundf(2 * vec_dot(obj->e3, vec) / obj->rad);
	if ((f2 + f3) % 2)
		return (obj->col1);
	return (obj->col2);
}
*/
