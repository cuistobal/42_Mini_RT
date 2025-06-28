#include "minirt.h"

void    *freeGenericPointer(void *ptr)
{
    if (ptr)
        free(ptr);
    return (NULL);
}

void	*freeArr(char **arr, int len)
{
	while (--len)
        arr[len] = freeGenericPointer(arr[len]);
	arr = freeGenericPointer(arr);
	return (arr);
}

//
void	*freeObject(t_object *ptr)
{
	if (ptr)
		ptr = freeGenericPointer(ptr);
	return (ptr);
}

void	*freeScene(t_scene *ptr)
{
	t_object	*next;

    if (!ptr)
        return (NULL);
    if (ptr->objects)
    {
		while (ptr->objects)
		{
        	next = ptr->objects->next;
        	freeObject(ptr->objects);
        	ptr->objects = next;
		}
    }
    ptr->light = freeObject(ptr->light);
    ptr->camera = freeObject(ptr->camera);
    ptr->ambiantLightning = freeObject(ptr->ambiantLightning);
	ptr = freeGenericPointer(ptr);
	return (ptr);
}

//
void	*freeMinirt(t_minirt *ptr)
{
	if (!ptr)
		return (NULL);
    ptr->scene = freeScene(ptr->scene);
	ptr->mlxptr = freeGenericPointer(ptr->mlxptr);  
	ptr->mlxwin = freeGenericPointer(ptr->mlxwin);
	return (NULL);
}
