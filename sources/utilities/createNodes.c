#include "minirt.h"

/*
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
    ft_bzero(new->colours, sizeof(long) * 4);
	new->next = NULL;
	return (new);
}
*/

t_scene *create_scene_node(void)
{
    t_scene *new;

    new = malloc(sizeof(t_scene));
    if (!new)
        return (NULL);
    new->light = NULL;
    new->camera = NULL;
    new->ambiant = NULL;
    new->objects = NULL;
    return (new);
}
