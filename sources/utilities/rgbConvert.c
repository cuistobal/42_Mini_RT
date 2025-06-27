#include "minirt.h"

static bool convertRGB(char *rgb, long *value)
{
	*value = ft_atoi(rgb);
    return (*value >= 0 && *value <= 255);
}

//static bool purgeWhitespaces()

bool    getColours(t_object **object, char *str)
{
    int     i;
    int     mask;
    int     colour;
    char    *param;

    i = 1;
    mask = 16;
    colour = 0;
    if (!str)
        return (false);
    while (i < 3)
    {
        if (!is_digit(*str))
            break ;
        param = ft_strtok_r(str, ",", &str);
        if (!convertRGB(param, &(*object)->colour[i]))
            break ;
        *(*object)->colour |= (*object)->colour[i] << mask;
        mask = mask - 8;
    }
    return (!*str);
}
