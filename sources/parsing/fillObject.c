#include "minirt.h"

//
static bool fillScenarioOne(t_object *object, char *str)
{
    //Ratio
    //
    //Colours
    char    *temp;
    bool    status;

    temp = ft_strtok_r(str, WHITESPACES, &str);
    status =  getCoordinates(object, str, convert, limits);
    //if (!temp || convertToFloatEnforceLimits(&(*(*object)->ratio), str, ALOWLIM, AMAXLIM))
      //  return (false);
    temp = ft_strtok_r(str, WHITESPACES, &str);
    return (!status || getColours(object, temp));
}

//
static bool fillScenarioTwo(t_object *object, char *str)
{
    (void)object;
    (void)str;
    if (!getCoordinates(object, str, convert, limits))
        return (false); 
    if (!getCoordinates(object, str, convert, limits))
        return (false);
    //Coord
    //
    //Vector
    //
    //Ratio
    return true;
}

//
static bool fillScenarioThree(t_object *object, char *str)
{
    (void)object;
    (void)str;
    //Coord
    //
    //Vector
    //
    //Ratio
    //
    //Ratio
    //
    //Colours
    return true;
}

//
bool	fillObject(t_object *object, char *str)
{
    char    type;

    type = object->type;
    if (type == EALIGHT)
        return (fillScenarioOne(object, str));
    if (type == ELIGHT || type == ESPHERE || type == ECAMERA)
        return (fillScenarioTwo(object, str));
    if (type == EPLANE || type == ECYLINDER)
        return (fillScenarioThree(object, str));
    return (false);
}
