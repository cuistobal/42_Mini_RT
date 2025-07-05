#include "minirt.h"

//
static int	count_objects_and_populate_aabb(t_object *objects)
{
	int	i;

	i = 0;
	while (objects)
	{
		objects->methods.bounds(objects);
		objects = objects->next;
		i++;
	}
	return (i);
}

//
bool	start_rendering(t_minirt *minirt)
{
	int			count;
	t_bvh		*root;
	t_object	*current;

	root = NULL;
	current = minirt->scene->objects;
	count = count_objects_and_populate_aabb(minirt->scene->objects);
//	if (!build_bvh(&root, current, count, 0))
//		return (false);		//probbaly needs some free here

		
	trace_rays(minirt, minirt->screen.width, minirt->screen.height);
		
	return (true);
}
