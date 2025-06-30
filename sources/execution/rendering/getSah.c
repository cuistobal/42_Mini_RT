#include <minirt.h>

//
static inline float	getCenter(t_object *current, int axis)
{
	if (axis == 0)
		return (current->coordinates[X]);
	else if (axis == 1)
		return (current->coordinates[Y]);
	return (current->coordinates[Z]);
}

//
static float	surfaceAreaDifference(t_aabb *lBounds, t_aabb *rBounds, int lCOunt, int rCOunt)
{
	float	lsa;
	float	rsa;

	lsa = getAabbSurfaceArea(lBounds);
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

	rsa = getAabbSurfaceArea(rBounds);
    return (0.125f + (lCOunt * lsa + rCOunt * rsa));
	//Change magic number. We could compute it but 0.125 is an accurate 
	//approximation and I'm too lazy to implement the formula :)
}

//
static float	evaluateSah(t_bvh *node, int axis, float split)
{
	int 		diff;
	int			center;
	t_aabb		*lBounds;
	t_aabb		*rBounds;
	t_object	*current;
    
	diff = 0;
	center = 0;
	current = node->objects;
    lBounds = createAabbNode(NULL);
    rBounds = createAabbNode(NULL);
	while (current)
	{
		center = getCenter(current, axis);
		if (center < split)
			combineAabbNodes(lBounds, createAabbNode(current));
		else
		{
			diff++;
			combineAabbNodes(rBounds, createAabbNode(current));
		}
		current = current->next;
	}
	return (surfaceAreaDifference(lBounds, rBounds, \
				node->objCount - diff, diff));
}

//
static float	computeSplit(t_bvh *node, int axis, int i)
{
	if (axis == 0)
    	return (node->bounds->minVec[X] + (i / 8.0f) * \
				(node->bounds->maxVec[X] - node->bounds->minVec[X]));
	else if (axis == 1)
    	return (node->bounds->minVec[Y] + (i / 8.0f) * \
				(node->bounds->maxVec[Y] - node->bounds->minVec[Y]));
   	return (node->bounds->minVec[Z] + (i / 8.0f) * \
		(node->bounds->maxVec[Z] - node->bounds->minVec[Z]));
}

//Returns the best axis
static int	getBestAxis(t_bvh *node, float *bestCost, float *bestSplit, int axis)
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
		split =	computeSplit(node, axis, i);
		cost = evaluateSah(node, axis, split);
		if (cost < *bestCost)
		{
			*bestCost = cost;
            ret = axis;
            *bestSplit = split;
		}
		i++;
	}
	return (ret);
}

//
void	getSah(t_bvh *node, int *bestAxis, float *bestSplit)
{
	int		axis;
    float 	bestCost;

	axis = 0;
	bestCost = INFINITY; // Find the value of this preset float
	while (axis < 3)
	{
		*bestAxis = getBestAxis(node, &bestCost, bestSplit, axis);
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
