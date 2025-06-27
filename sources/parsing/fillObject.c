#include "minirt.h"

//
static bool fillScenarioOne(t_object **object, char *str)
{
    //Ratio
    //
    //Colours
    char    *temp;

    temp = ft_strtok_r(str, WHITESPACES, &str);
    if (!temp || convertToFloatEnforceLimits(&(*(*object)->ratio), str, ALOWLIM, AMAXLIM))
        return (false);
    temp = ft_strtok_r(str, WHITESPACES, &str);
    return (getColours(object, temp));
}

//
static bool fillScenarioTwo(t_object **object, char *str)
{
    //Coord
    //
    //Vector
    //
    //Ratio
}

//
static bool fillScenarioThree(t_object **object, char *str)
{
    //Coord
    //
    //Vector
    //
    //Ratio
    //
    //Ratio
    //
    //Colours
}

//
bool	fillObject(t_object **object, char *str)
{
    char    type;

    type = (*object)->type;
    if (type == EALIGHT)
        return (fillScenarioOne(object, str));
    if (type == ELIGHT || type == ESPHERE | type == ECAMERA)
        return (fillScenarioTwo(object, str));
    if (type == EPLANE || type == ECYLINDER)
        return (fillScenarioThree(object, str));
    return (false);
}
