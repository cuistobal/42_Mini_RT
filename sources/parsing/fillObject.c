#include "minirt.h"

//Parsing function for the ambiant lightning
static bool fillScenarioOne(t_object *object, char **str)
{
    char    *temp;

    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!getRatio(&object->ratio[0], &temp, convert, true))
        return (false); 
    temp = ft_strtok_r(*str, WHITESPACES, str);
    return (getColours(object, &temp));
}

//Parsing function for planes, cylinders and cameras
static bool fillScenarioTwo(t_object *object, char **str, char type)
{
    if (!getCoordinates(object, str, convert, NULL))
        return (false); 
    if (!getCoordinates(object, str, convert, checkLimits))
        return (false);
    if (type == ECAMERA && getRatio(&object->ratio[0], str, convert, false))
        return (checkLimits(true, *object->ratio, RATIOLIMIN, FOVCOEF));
    else if (type == EPLANE)
        return (getColours(object, str));
    if (!getRatio(&object->ratio[0], str, convert, false))
        return (false);
    if (!getRatio(&object->ratio[1], str, convert, false))
        return (false);
    return (getColours(object, str));
}

//Parsing function for light and spheres
static bool fillScenarioThree(t_object *object, char **str, char type)
{
    if (!getCoordinates(object, str, convert, NULL))
        return (false);
    if (type == ELIGHT && !getRatio(&object->ratio[0], str, convert, true))
        return (false);
    else if (type != ELIGHT && !getRatio(&object->ratio[0], str,\
                convert, false))
        return (false);
    return (getColours(object, str));
}

//
bool	fillObject(t_object *object, char *str)
{
    char    type;

    type = object->type;
    if (type == EALIGHT)
        return (fillScenarioOne(object, &str));
    if (type == EPLANE || type == ECYLINDER || type == ECAMERA)
        return (fillScenarioTwo(object, &str, type));
    if (type == ELIGHT || type == ESPHERE)
        return (fillScenarioThree(object, &str, type));
    return (false);
}
