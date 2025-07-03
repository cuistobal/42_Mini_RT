#include "minirt.h"

//
static float	get_center(t_object *current, int axis)
{
	if (axis == 0)
		return (current->pdata.center.x);
	else if (axis == 1)
		return (current->pdata.center.y);
	return (current->pdata.center.z);
}

//
static int	split_object_list(t_object *list, t_object **mid, int baxis, float bsplit)
{
	t_object	*prev;
	t_object	*curr;	
	int			center;
	int			mcount;

	curr = list;
	*mid = list;
	prev = NULL;
	mcount = 0;
	while (curr)
	{
		center = get_center(curr, baxis);
        if (center < bsplit)
		{
			prev = *mid;
			*mid = (*mid)->next;
			mcount++;
		}
		curr = curr->next;
	}
	//Splitting the list arounf the middle node. headd ans mid will be 
	//different since we don't reach this aprt of the program if the list size
	//is not greater than 1,
	if (prev)
		prev->next = NULL;
	return (mcount);
}

//
static bool	create_bvh_node(t_bvh **node)
{
	if (*node)
		return (print_error_message(INVADRR));
	*node = malloc(sizeof(t_bvh));
	if (!node)
		return (print_error_message(MEMALLOC));
    (*node)->bounds = create_aabb_node(NULL);
    (*node)->left = NULL; 
	(*node)->right = NULL;
    (*node)->objects = NULL;
	(*node)->objcount = 0;
	return (true);
}

//
bool	build_bvh(t_bvh **root, t_object *objects, int count, int depth)
{
	t_object	*mid;
	t_object	*current;
	int			mid_count;
	int			best_axis;
	float		best_split;

	mid = NULL;
	best_axis = 0;
	best_split = 0;
	current = objects;
	if (!create_bvh_node(root))
		return (false);
	while (current)
	{
		combine_aabb_nodes((*root)->bounds, create_aabb_node(current));
		current = current->next;
	}	
	if (count <= 1 || depth >= 40) 
		return ((*root)->objects = objects, (*root)->objcount = count, true);
	get_sah(*root, &best_axis, &best_split);
	mid_count = split_object_list(objects, &mid, best_axis, best_split);
    if (!build_bvh(&(*root)->left, objects, count - mid_count , depth + 1))
		return (false);
	return (build_bvh(&(*root)->right, mid, mid_count, depth + 1));
}
