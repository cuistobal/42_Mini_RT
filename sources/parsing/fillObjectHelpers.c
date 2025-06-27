#include "minirt.h"

//
bool	convertToFloatLimitless(float *coordinates, char *str)
{
    *coordinates = ft_atof(str);
}

//
bool	convertToFloatEnforceLimits(float *ratio, char *str, float min, float max)
{
    *ratio = my_atof(str);
    return (*ratio >= min && *ratio <= max);
}
