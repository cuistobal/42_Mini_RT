#include "minirt.h"

//
static bool	leafScenario(t_bvh **node, t_object *objects, int count)
{
	if (count > 1 && depth < 40)  //fix Magic number
		return (false);
	(*node)->objects = objects;
	(*node)->count = count;
	return (true);
}

static t_object	getMiddleNode(t_object *list, int *midCount)
{
	t_object	*slow;
	t_object	*fast;

	slow = list;
	fast = slow->next;	
	(*midCount)--;
	while (fast && fast->next)
	{
		(*midCount)--;
		slow = slow->next;
		fast = fast->next->next;
	}
	return (slow);
}

static float	getCenter(t_object *current, int axis)
{
	if (axis == 0)
		return (current->coordinates[X]);
	else if (axis == 1)
		return (current->coordinates[Y]);
	return (current->coordinates[Z]);
}

//
static void splitObjectList(t_object *list, t_object **mid, int *midCount, float bestSplit)
{
	t_object	*mid;
	t_object	*temp;
	t_object	*prev;
	t_object	*curr;	
	int			center;

	curr = list;
	*mid = list;
	prev = NULL;
	while (curr)
	{
		center = getCenter(curr, bestAxis)
        if (center < bestSplit)
		{
			prev = *mid;
			*mid = (*mid)->next;
		}
		curr = curr->next;
	}
	//Splitting the list arounf the middle node. headd ans mid will be 
	//different since we don't reach this aprt of the program if the list size
	//is not greater than 1,
	if (prev)
		prev->next = NULL;
}

static bool	createBvhNode(t_bvh **node, t_object *objects, int count)
{
	if (*node)
		return (printf("Node is already allocated\n"), false);
	*node = malloc(sizeof(t_bvh));
	if (!node)
		return (false);
    node->bounds = create_empty_aabb();
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
	int			midcount;
	int			bestAxis;
	float		bestSplit;

	mid = NULL;
	bestAxis = 0;
	bestSplit = 0;
	current = objects;
	if (!createBvhNode(root, objects, count))
		return (false);
    node->bounds = create_empty_aabb();
	while (current)
	{
		node->bounds = combineAabb(node->bounds, createAabbFromObject(current));
		current = current->next;
	}	
	if (count > 1 && depth < 40) 
		return ((*root)->objects = objects, (*root)->count = count, true);
	getSah(node, &bestAxis, &bestSplit);
	midCount = count;
	splitObjectList(objects, &mid, &midCount, bestAxis);
    node->left = build_bvh_node(spheres, objects, count - midCount , depth + 1);
    node->right = build_bvh_node(spheres, mid, midCount, depth + 1);
	return (true);
}
