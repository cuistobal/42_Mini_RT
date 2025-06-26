#include "minirt.h"

void	*freeArr(char **arr, int len)
{
	char	*curr;

	while (--len)
	{
		free(arr[len])
		arr[len] = NULL;
	}
	free(arr);
	arr = NULL;
	return (arr);
}

//
void	*freeObject(t_object *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
	return (ptr);
}

void	*freeScene(t_scene *ptr)
{
	t_object	*next;

	if (!scene)
		return ;
	while (scene->objects)
	{
		next = scene->objects->next;
		free(scene->objects);
		scene->objects = NULL;
	}
	free(ptr);
	ptr = NULL;
	return (ptr);
}

//
void	*freeMinirt(t_minirt *ptr)
{
	if (!ptr)
		return ;
	if (ptr->scene)
	{
		freeScene(ptr->scene);
		ptr->scene = NULL;
	}
	if (ptr->mlxptr)
	{
	}
	if (ptr->mlxwin)
	{
	}
/*
 *	DOuble free() possible since objects are freed in freeSene();
 *
	while (ptr->object)
	{
		next = ptr->object->next;
		freeObject(ptr->objects);
		ptr->objects = next;
	}
*/
	free(ptr);
	ptr = NULL;
	return (ptr);
}
