#include "minirt.h"

//
void	insertNewObject(t_object **head, t_object **tail, t_object *new)
{
	if (!*head)
	{
		*head = new;
		*tail = new;
	}
	else
	{
		(*tail)->next = new;
		*tail = (*tail)->next;
	}
}

//
void	*createObjectNode(void)
{
	t_object	*new;

	new = malloc(sizeof(t_object));
	if (!new)
		return (NULL);
	new->type = 0;
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

