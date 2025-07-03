#include "minirt.h"

void    *free_generic_pointer(void *ptr)
{
    if (ptr)
        free(ptr);
    return (NULL);
}

void	*free_arr(char **arr, int len)
{
	while (--len)
        arr[len] = free_generic_pointer(arr[len]);
	arr = free_generic_pointer(arr);
	return (arr);
}

//
void	*free_object(t_object *ptr)
{
	if (ptr)
		ptr = free_generic_pointer(ptr);
	return (ptr);
}

void	*free_scene(t_scene *ptr)
{
	t_object	*next;

    if (!ptr)
        return (NULL);
    if (ptr->objects)
    {
		while (ptr->objects)
		{
        	next = ptr->objects->next;
        	free_object(ptr->objects);
        	ptr->objects = next;
		}
    }
    ptr->light = free_object(ptr->light);
    ptr->camera = free_object(ptr->camera);
    ptr->ambiant= free_object(ptr->ambiant);
	ptr = free_generic_pointer(ptr);
	return (ptr);
}

//
void	*free_minirt(t_minirt *ptr)
{
	if (!ptr)
		return (NULL);
    ptr->scene = free_scene(ptr->scene);
	ptr->mlxptr = free_generic_pointer(ptr->mlxptr);  
	ptr->mlxwin = free_generic_pointer(ptr->mlxwin);
	return (NULL);
}
