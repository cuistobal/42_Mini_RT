#include "minirt.h"

//
static int	countObjectsAndAppendAabb(t_object *objects)
{
	int	i;

	i = 0;
	while (objects)
	{
		objects = objects->next;
		i++;
	}
	return (i);
}

//
bool	startRendering(t_minirt *minirt)
{
	int			count;
	t_bvh		*root;
	t_object	*current;

	root = NULL;
	current = minirt->scene->objects;
	count = countObjectsAndAppendAabb(current);
	if (!buildBvh(&root, current, count, 0))
		return (false);		//probbaly needs some free here

	printf("isokemafriend\n");
	printTreeBFS(root);

	return (true);
}
