#include "minirt.h"

//
static float	getCenter(t_object *current, int axis)
{
	if (axis == 0)
		return (current->coordinates[X]);
	else if (axis == 1)
		return (current->coordinates[Y]);
	return (current->coordinates[Z]);
}

//
static int	splitObjectList(t_object *list, t_object **mid, int bestAxis, float bestSplit)
{
	t_object	*prev;
	t_object	*curr;	
	int			center;
	int			midCount;

	curr = list;
	*mid = list;
	prev = NULL;
	midCount = 0;
	while (curr)
	{
		center = getCenter(curr, bestAxis);
        if (center < bestSplit)
		{
			prev = *mid;
			*mid = (*mid)->next;
			midCount++;
		}
		curr = curr->next;
	}
	//Splitting the list arounf the middle node. headd ans mid will be 
	//different since we don't reach this aprt of the program if the list size
	//is not greater than 1,
	if (prev)
		prev->next = NULL;
	return (midCount);
}

//
static bool	createBvhNode(t_bvh **node)
{
	if (*node)
		return (printErrorMessage(INVADRR));
	*node = malloc(sizeof(t_bvh));
	if (!node)
		return (printErrorMessage(MEMALLOC));
    (*node)->bounds = createAabbNode(NULL);
    (*node)->left = NULL; 
	(*node)->right = NULL;
    (*node)->objects = NULL;
	(*node)->objCount = 0;
	return (true);
}

//
bool	buildBvh(t_bvh **root, t_object *objects, int count, int depth)
{
	t_object	*mid;
	t_object	*current;
	int			midCount;
	int			bestAxis;
	float		bestSplit;

	mid = NULL;
	bestAxis = 0;
	bestSplit = 0;
	current = objects;
	if (!createBvhNode(root))
		return (false);
	while (current)
	{
		combineAabbNodes((*root)->bounds, createAabbNode(current));
		current = current->next;
	}	
	if (count <= 1 || depth >= 40) 
		return ((*root)->objects = objects, (*root)->objCount = count, true);
	getSah(*root, &bestAxis, &bestSplit);
	midCount = splitObjectList(objects, &mid, bestAxis, bestSplit);
    if (!buildBvh(&(*root)->left, objects, count - midCount , depth + 1))
		return (false);
	return (buildBvh(&(*root)->right, mid, midCount, depth + 1));
}
