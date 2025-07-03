#include <minirt.h>

//
static inline float	get_center(t_object *current, int axis)
{
	if (axis == 0)
		return (current->pdata.center.x);
	else if (axis == 1)
		return (current->pdata.center.y);
	return (current->pdata.center.z);
}

//
static float	surface_area_difference(t_aabb *l, t_aabb *r, int lc, int rc)
{
	float	lsa;
	float	rsa;

	lsa = get_aabb_surface_area(l);
	//
/*float get_aabb_surface_area(AABB box)
{
    Vec3 dimensions = {
        box.max.x - box.min.x,
        box.max.y - box.min.y,
        box.max.z - box.min.z};
    return 2.0f * (dimensions.x * dimensions.y +
                   dimensions.y * dimensions.z +
                   dimensions.z * dimensions.x);
}*/

	//I think it's the vec product of the max and mion value within the box

	rsa = get_aabb_surface_area(r);
    return (0.125f + (lc * lsa + rc * rsa));
	//Change magic number. We could compute it but 0.125 is an accurate 
	//approximation and I'm too lazy to implement the formula :)
}

//
static float	evaluate_sah(t_bvh *node, int axis, float split)
{
	int 		diff;
	int			center;
	t_aabb		*lbounds;
	t_aabb		*rbounds;
	t_object	*current;
    
	diff = 0;
	center = 0;
	current = node->objects;
    lbounds = create_aabb_node(NULL);
    rbounds = create_aabb_node(NULL);
	while (current)
	{
		center = get_center(current, axis);
		if (center < split)
			combine_aabb_nodes(lbounds, create_aabb_node(current));
		else
		{
			diff++;
			combine_aabb_nodes(rbounds, create_aabb_node(current));
		}
		current = current->next;
	}
	return (surface_area_difference(lbounds, rbounds, \
				node->objcount - diff, diff));
}

//
static float	compute_split(t_bvh *node, int axis, int i)
{
	if (axis == 0)
    	return (node->bounds->min_vec.x + (i / 8.0f) * \
				(node->bounds->max_vec.x - node->bounds->min_vec.x));
	else if (axis == 1)
    	return (node->bounds->min_vec.y + (i / 8.0f) * \
				(node->bounds->max_vec.y - node->bounds->min_vec.y));
   	return (node->bounds->min_vec.z + (i / 8.0f) * \
		(node->bounds->max_vec.z - node->bounds->min_vec.z));
}

//Returns the best axis
static int	get_best_axis(t_bvh *node, float *bcost, float *bsplit, int axis)
{
	int		i;
	int		ret;
	float	cost;
	float	split;

	i = 1;
	ret = 0;
	cost = 0;
	while (i < 8)
	{
		split =	compute_split(node, axis, i);
		cost = evaluate_sah(node, axis, split);
		if (cost < *bcost)
		{
			*bcost = cost;
            ret = axis;
            *bsplit = split;
		}
		i++;
	}
	return (ret);
}

//
void	get_sah(t_bvh *node, int *best_axis, float *best_split)
{
	int		axis;
    float 	best_cost;

	axis = 0;
	best_cost = INFINITY; // Find the value of this preset float
	while (axis < 3)
	{
		*best_axis = get_best_axis(node, &best_cost, best_split, axis);
		axis++;
	}
}


/*	ORIGINAL VERSION
    float best_cost = INFINITY;
    int best_axis = 0;
    float best_split = 0;

    for (int axis = 0; axis < 3; axis++)
    {
        for (int i = 1; i < 8; i++)
        {
            float split;
            if (axis == 0)
            {
                split = node->bounds.min.x + (i / 8.0f) * (node->bounds.max.x - node->bounds.min.x);
            }
            else if (axis == 1)
            {
                split = node->bounds.min.y + (i / 8.0f) * (node->bounds.max.y - node->bounds.min.y);
            }
            else
            {
                split = node->bounds.min.z + (i / 8.0f) * (node->bounds.max.z - node->bounds.min.z);
            }

            float cost = evaluate_sah(spheres, start, end, axis, split);

            if (cost < best_cost)
            {
                best_cost = cost;
                best_axis = axis;
                best_split = split;
            }
        }
    }
*/
