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

static inline void	appendPointers(t_object **list)
{
	t_object	*temp;

	temp = *list;
	if (!temp)
		return ;
	*list = temp->next;
	temp->next = NULL;
}

//
static void splitObjectList(t_object *list, t_object **mid, int *midCount)
{
	t_object	*mid;
	t_object	*temp;
	t_object	*curr;	
	int			center;

	curr = list;
	*mid = getMiddleNode(objects, &midCount);	
	while (curr)
	{
		if (bestAzis == 0)
            center = curr.center.x;
		else if (bestAzis == 1)
            center = curr.center.y;
		else
            center = curr.center.z;
        if (center < best_split)
        {
            temp = curr;
            curr = mid;
			mid = temp->next;
        }
		current = current->next;
	}
	appendPointers(&mid);
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
	float		sah;
	t_object	*mid;
	t_object	*current;
	int			midcount;

	mid = NULL;
	current = objects;
	if (!createBvhNode(root, objects, count))
		return (leafScenario(root, objects, count));
    node->bounds = create_empty_aabb();
	while (current)
	{
		node->bounds = combineAabb(node->bounds, createAabbFromObject(current));
		current = current->next;
	}	
	if (leafScenario(&node))
		return (node);
	sah = getSah(node);
	midCount = count;
	current = objects;
	splitObjectList(current, &mid, &midCount);
    node->left = build_bvh_node(spheres, current, count - midCount , depth + 1);
    node->right = build_bvh_node(spheres, mid, midCount, depth + 1);
    return (node);
}
