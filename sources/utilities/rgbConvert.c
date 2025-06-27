#include "minirt.h"

static bool convertRGB(char *rgb, long *value)
{
	*value = ft_atoi(rgb);
    return (*value >= 0 && *value <= 255);
}


bool    getColours(t_object **object, char *str)
{
    int i;
    int mask;
    int colour;

    i = 1;
    mask = 16;
    colour = 0;
    while (i < 3)
    {
        if (!is_digit(*str))
            break ;
        if (!convertRGB(str, &(*object)->colour[i]))
            break ;
        *(*object)->colour |= (*object)->colour[i] << mask;
        mask = mask - 8;
        str++;
    }
    return (!*str);
}
