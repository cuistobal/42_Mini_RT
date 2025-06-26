#include "minirt.h"

void	fillCoordinates(t_object **object, char *str)
{

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
}
