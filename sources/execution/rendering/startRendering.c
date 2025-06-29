#include "minirt.h"

void	printTreeBFS(t_bvh *root)
{
	if (!root)
		return ;
	printTreeBFS(root->left);
	printTreeBFS(root->right);
	printObject(root->objects);
}

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
	if (!buildBvh(&root, current, count, 0);	
		return (false);		//probbaly needs some free here

	printTreeBFS(root);

	return (true);
}
