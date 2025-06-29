#include "minirt.h"

t_aabb	*combineAabbNodes(t_aabb *node1, t_aabb *node2)
{
	return NULL;			
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
