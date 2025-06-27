#include "minirt.h"

//
bool	convertToFloatLimitless(float *coordinates, char *str)
{
    bool    error;

    error = true;
    *coordinates = ft_atof(str, &error);
    return (error);
}

//
bool	convertToFloatEnforceLimits(float *ratio, char *str, float min, float max)
{
    bool    error;

    error = true;
    *ratio = ft_atof(str, &error);
    return (error && *ratio >= min && *ratio <= max);
}
