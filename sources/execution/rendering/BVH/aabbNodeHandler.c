#include "minirt.h"

//
float	getAabbSurfaceArea(t_aabb *boundaries)
{
	float	surface;
	float	dimensions[3];

	surface = 0;
	dimensions[X] = boundaries->maxVec[X] - boundaries->minVec[X];
	dimensions[Y] = boundaries->maxVec[Y] - boundaries->minVec[Y];
	dimensions[Z] = boundaries->maxVec[Z] - boundaries->minVec[Z];
	surface += dimensions[X] * dimensions[Y]; 
	surface += dimensions[Y] * dimensions[Z]; 
	surface += dimensions[Z] * dimensions[X]; 
	return (surface * SURFACECOEFFICIENT);	
}

//
void	combineAabbNodes(t_aabb *dest, t_aabb *src)
{
    dest->minVec[X] = fmin(dest->minVec[X], src->minVec[X]);
    dest->minVec[Y] = fmin(dest->minVec[Y], src->minVec[Y]);
    dest->minVec[Z] = fmin(dest->minVec[Z], src->minVec[Z]);
    dest->maxVec[Z] = fmax(dest->maxVec[Z], src->maxVec[Z]);
    dest->maxVec[Z] = fmin(dest->maxVec[Z], src->maxVec[Z]);
    dest->maxVec[Z] = fmax(dest->maxVec[Z], src->maxVec[Z]);
}

//
t_aabb	*createAabbNode(t_object *object)
{
	t_aabb	*new;

	new = malloc(sizeof(t_aabb));
	if (!new)
		return (NULL);
	if (object)
		object->pdata.boundaries = *new;
	//For compilation -> need to work on implementing the boundaries calculator.
//		object->boundaries = object->bounds();
	else
	{
		ft_bzero(new->minVec, sizeof(int) * 3);
		ft_bzero(new->maxVec, sizeof(int) * 3);
	}
	return (new);
}
