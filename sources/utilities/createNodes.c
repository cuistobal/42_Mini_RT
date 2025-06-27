#include "minirt.h"

//
t_object    *createObjectNode(void)
{
	t_object	*new;

	new = malloc(sizeof(t_object));
	if (!new)
		return (NULL);
	new->type = 0;
    ft_bzero(new->ratio, sizeof(float) * 2);
    ft_bzero(new->coordinates, sizeof(float) * 3);
    ft_bzero(new->normVector, sizeof(float) * 3);
    ft_bzero(new->colour, sizeof(float) * 4);
	new->next = NULL;
	return (new);
}

//
void	*createMinirtNode(void)
{
	t_minirt	*new;

	new = malloc(sizeof(t_minirt));
	if (!new)
		return (NULL);
	new->scene = NULL;
	new->mlxptr = NULL;
	new->mlxwin = NULL;
	new->objects = NULL;
	return (new);
}

