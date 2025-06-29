#include "minirt.h"

t_aabb	*combineAabbNodes(t_aabb *node1, t_aabb *node2)
{
	
        fmin(a.min.x, b.min.x),
         fmin(a.min.y, b.min.y),
         fmin(a.min.z, b.min.z)},
        {fmax(a.max.x, b.max.x),
         fmax(a.max.y, b.max.y),
         fmax(a.max.z, b.max.z)}};
}

t_aabb	*createAabbNode(t_object *object)
{
	t_aabb	*new;

	new = malloc(sizeof(t_aab));
	if (!new)
		return (NULL);
	if (!object)
	{
		ft_bzero(new->minVec, sizeof(int) * 3);
		ft_bzero(new->maxVec, sizeof(int) * 3);
	}
	return (new);
}
