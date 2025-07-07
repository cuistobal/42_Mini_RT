#include <minirt.h>

//
static inline float	get_center(t_object *current, int axis)
{
	if (!current)
		return (0);
	if (axis == 0)
		return (current->pdata.center.x);
	else if (axis == 1)
		return (current->pdata.center.y);
	return (current->pdata.center.z);
}

//
static float	surface_area_difference(t_aabb parent, t_aabb l, t_aabb r, int lc, int rc)
{
	float	lsa;
	float	rsa;
	float   psa;
	float   traversal_cost;
	float   intersection_cost;

	psa = get_aabb_surface_area(parent);
	lsa = get_aabb_surface_area(l);
	rsa = get_aabb_surface_area(r);

	// Traversal cost for BVH node (typically much less than intersection cost)
	traversal_cost = 1.0f;
	
	// Intersection cost (typically much higher than traversal)
	intersection_cost = 8.0f;

	// SAH = traversal_cost + (p_left * n_left + p_right * n_right) * intersection_cost
	// where p_left and p_right are probabilities of hitting child nodes
	return (traversal_cost + ((lsa / psa) * lc + (rsa / psa) * rc) * intersection_cost);
}

//
static float	evaluate_sah(t_bvh *node, int axis, float split)
{
	int 		diff;
	int			center;
	t_aabb		lbounds;
	t_aabb		rbounds;
	t_object	*current;
    
	diff = 0;
	center = 0;
	current = node->objects;
	lbounds = set_aabb_value(set_vec_value(0, 0, 0), set_vec_value(0, 0, 0));
	rbounds = set_aabb_value(set_vec_value(0, 0, 0), set_vec_value(0, 0, 0));
	while (current)
	{
		center = get_center(current, axis);
		if (center < split)
			combine_aabb_nodes(lbounds, current->pdata.boundaries);
		else
		{
			diff++;
			combine_aabb_nodes(rbounds, current->pdata.boundaries);
		}
		current = current->next;
	}
	return (surface_area_difference(node->bounds, lbounds, rbounds, \
				node->objcount - diff, diff));
}

//
static float compute_split(t_object *objects, int axis)
{
    t_vec min = {1e30f, 1e30f, 1e30f};
    t_vec max = {-1e30f, -1e30f, -1e30f};
    int count = 0;
    t_object *curr = objects;

    // First pass: find actual bounds of objects
    while (curr)
    {
        float val = get_center(curr, axis);
        if (val < (axis == 0 ? min.x : axis == 1 ? min.y : min.z))
            *(axis == 0 ? &min.x : axis == 1 ? &min.y : &min.z) = val;
        if (val > (axis == 0 ? max.x : axis == 1 ? max.y : max.z))
            *(axis == 0 ? &max.x : axis == 1 ? &max.y : &max.z) = val;
        count++;
        curr = curr->next;
    }

    // If all objects are at the same position on this axis, move to next axis
    if ((axis == 0 && min.x == max.x) ||
        (axis == 1 && min.y == max.y) ||
        (axis == 2 && min.z == max.z))
        return (axis == 0 ? min.x : axis == 1 ? min.y : min.z);

    // Choose split position as median of actual object positions
    curr = objects;
    float midPoint = count / 2;
    int i = 0;
    while (curr && i < midPoint)
    {
        curr = curr->next;
        i++;
    }
    
    return get_center(curr, axis);
}

//Returns the best axis
static int	get_best_axis(t_bvh *node, float *bcost, float *bsplit, int curr_axis)
{
	float	cost;
	float	split;

	split = compute_split(node->objects, curr_axis);
	cost = evaluate_sah(node, curr_axis, split);
	if (cost < *bcost)
	{
		*bcost = cost;
		*bsplit = split;
		return (curr_axis);
	}
	return (0);
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
