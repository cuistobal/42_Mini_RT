#include <minirt.h>

static inline float	getCenter(t_object *current)
{
	if (axis == 0)
		return (current->coordinates[X]);
	else if (axis == 1)
		return (current->coordinates[Y]);
	return (current->coordinates[Z]);
}



static float	surfaceAreaDifference(t_aabb *lBounds, t_aabb *rBounds, int lCOunt, int rCOunt)
{
	float	lsa;
	float	rsa;

	lsa = getAaabSurfaceArea(lBounds);
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

	rsa = getAaabSurfaceArea(rBounds);
    return (0.125f + (lCOunt * lsa + rCOunt * rsa));
	//Change magic number. We could compute it but 0.125 is an accurate 
	//approximation and I'm too lazy to implement the formula :)
}

//
static float	evaluateSah(t_object *objects, int count, int axis, float split)
{
	int 		diff;
	t_aabb		lBounds;
	t_aabb		rBounds;
	t_object	*current;
    
	diff = 0;
	current = objects;
    lBounds = create_empty_aabb();
    rBounds = create_empty_aabb();
	while (current)
	{
		center = getCenter(current);
		if (center < split)
			lBounds = combineAabb(lBounds, createAabbNode(current));
		else
		{
			diff++;
			rBounds = combineAabb(rBounds, createAabbNode(current));
		}
		current = current->next;
	}
	return (surfaceAreaDifference(lBounds, rBounds, count - diff, diff))	
}

//
static float	computeSplit(t_bvh *node, int axis, int i)
{
	if (axis == 0)
    	return (node->bounds->minVec[X] + (i / 8.0f) * \
				(node->bounds->max[X] - node->bounds->minVec[X]);
	else if (axis == 1)
    	return (node->bounds->minVec[Y] + (i / 8.0f) * 
			\(node->bounds->max[Y] - node->bounds->minVec[Y]);
   	return (node->bounds->minVec[Z] + (i / 8.0f) * \
		(node->bounds->max[Z] - node->bounds->minVec[Z]);
}

//Returns the best axis
static int	getBestAxis(t_bvh *node, int *bestCost, int *bestSplit, int axis)
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
		computeSplit(node, axis, i);
		cost = evaluateSah();
		if (cost < *bestCost)
		{
			*bestCost = cost;
            ret = axis;
            *best_split = split;
		}
		i++;
	}
	return (ret);
}

//
void	getSah(t_bvh *node, int *bestAxis, int *bestSplit)
{
	int		axis;
    int		bestAxis;
    float 	bestCost;
    float	bestSplit;

	axis = 0;
	bestAxis = 0;
	bestCost = INFINITY; // Find the value of this preset float
	bestSplit = 0;
	while (axis < 3)
	{
		bestAxis = getBestAxis(node, &bestCost, &bestSplit, axis);
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
