#include "minirt.h"

//
bool	convert(float *coordinates, char *str)
{
    bool    error;

    error = true;
    *coordinates = ft_atof(str, &error);
    return (error);
}

//
bool	checkLimits(bool status, float value, float min, float max)
{
    return (status && value >= min && value <= max);
}

//
bool    getRatio(float *value, char **str, bool (*convert)(float *dst, \
            char *src), bool limits)
{ 
    char    *temp;
    bool    status;

    status = true;
    temp = ft_strtok_r(*str, WHITESPACES, str);
    status = convert(value, temp);
    if (limits)
        return (status && checkLimits(status, *value, RATIOLIMIN, RATIOLIMAX));
    return (status);
}

//
bool    getCoordinates(t_object *obj, char **str, bool (*convert)(float *dst, \
            char *src), bool (*checkLimits)(bool s, float i, float j, float k))
{
    int     i;
    char    *temp;
    char    *param;
    bool    status;

    i = 0;
    status = true;
    temp = ft_strtok_r(*str, WHITESPACES, str);
    while (status && i++ < 3)
    {
        param = ft_strtok_r(temp, ",", &temp);
        status = convert(&obj->coordinates[i], param);
        if (checkLimits)
            status = checkLimits(status, obj->coordinates[i], \
                    VECLIMIN, VECLIMAX);
    }
    return (status && i == 4);
}
