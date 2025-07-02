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

    i = 1;
    mask = 16;
	status = !(!str || !*str);
    while (status && i++ < 4)
    {
        status = isdigit(**str);
        param = ft_strtok_r(*str, ",", str);
       	status = convertRGB(param, &temp[i]);
        *colors |= temp[i] << mask;
        mask = mask - 8;
    }
    return (status);
}
