#include "minirt.h"

static bool	fillCoordinates(t_object **object, char *str)
{
	int		i;
	int		j;
	float	coordinates[3];

	j = 0;
	i = 0;
	while (str[i] && j++ < 3)
	{
		extractFloat(str, &i, coordinates, j);
	}
	*object->coordinates = coordinates;
}

static bool	fillLightingRatio(t_object **object, char *str)
{
	float	ratio;

	if (ratio < 0 || ratio > 1)
		return (false);
	return (*object->coordinates[0] = ratio, true);
}


//
bool	fillObject(t_object **object, char *str)
{
	char	*tmp;

	tmp = ft_strtok_r(line, "WHITESPACES", &line);
	if (!tmp)
		return (false);
	if ((*object)->type != 'A')
		fillCoordinates(object, str)
	else
		fillLightingRation(object, str);	
}
