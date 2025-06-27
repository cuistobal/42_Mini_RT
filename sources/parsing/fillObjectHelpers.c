#include "minirt.h"

//
bool	fillCoordinates(t_object **object, char *str)
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

//
bool	fillLightingRatio(t_object **object, char *str)
{
	float	ratio;

    ratio = my_atof(str);
	if (ratio < 0 || ratio > 1)
		return (false);
	return (*object->coordinates[0] = ratio, true);
}

