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
bool	limits(bool status, float value, float min, float max)
{
    return (status && value >= min && value <= max);
}

static float    getObjLimits(char type)
{
    (void)type;
    return 0.0;
}

bool    getCoordinates(t_object *obj, char *str, bool (*convert)(float *dst, \
            char *src), bool (*limits)(bool s, float i, float j, float k))
{
    int     i;
    char    *temp;
    char    *param;
    bool    status;

    i = 0;
    status = true;
    temp = ft_strtok_r(str, WHITESPACES, &str);
    while (status && i++ < 4)
    {
        param = ft_strtok_r(temp, ",", &temp);
        status = convert(&obj->coordinates[i], param);
        if (limits)
            status = limits(status, obj->coordinates[i], \
                    getObjLimits(obj->type), getObjLimits(obj->type));
    }
    return (status && i == 4);
}
