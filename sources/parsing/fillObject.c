#include "minirt.h"

static bool	endOfScenario(char *str)
{
	while (*str && isspace(*str))
		str++;
	return (!*str);
}

//Parsing function for the ambiant lightning
static bool fillScenarioOne(t_object *object, char **str)
{
    char    *temp;

    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!getRatio(&object->ratio[0], &temp, convert, true))
        return (false); 
    temp = ft_strtok_r(*str, WHITESPACES, str);
    if (!getColours(object, &temp))
		return (false);
	return (endOfScenario(*str));
}

//Parsing function for planes, cylinders and cameras
static bool fillScenarioTwo(t_object *object, char **str, char type)
{
    if (!getCoordinates(object, str, convert, NULL))
        return (false); 
    if (!getCoordinates(object, str, convert, checkLimits))
        return (false);
    if (type == ECAMERA)
	{
		if (!getRatio(&object->ratio[0], str, convert, false) || !checkLimits \
				(true, *object->ratio, RATIOLIMIN, FOVCOEF))
			return (false);
		return (endOfScenario(*str));
	}
    else if (type == EPLANE)
	{
        if (!getColours(object, str))
			return (false);
		return (endOfScenario(*str));
	}
    if (!getRatio(&object->ratio[0], str, convert, false) || !getRatio \
			(&object->ratio[1], str, convert, false))
		return (false);
	else if (!getColours(object, str))
		return (false);
	return (endOfScenario(*str));
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
    if (!getColours(object, str))
		return (false);
	return (endOfScenario(*str));
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
