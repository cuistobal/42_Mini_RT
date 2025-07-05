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
void	*free_bvh(t_bvh *root)
{
	if (!root)
		return (NULL);
	root->left = free_bvh(root->left);
	root->right = free_bvh(root->left);
	free(root);
	root = NULL;
	return (NULL);
}

//
void	*free_cache(t_cache *cache)
{	
	t_cache	*head;
	t_cache	*current;

	head = cache;
	current = cache;
	while (current && current->next != head)
	{
		current->prev = free_generic_pointer(current->prev);
		current = current->next;
	}
	free_generic_pointer(current);
	return (NULL);
}

//
void	free_rendering(t_render *ptr)
{
	ptr->lru = free_cache(ptr->lru);
	ptr->root = free_bvh(ptr->root);
}

//
void	*free_minirt(t_minirt *ptr)
{
	if (!ptr)
		return (NULL);
    ptr->scene = free_scene(ptr->scene);
	free_rendering(&ptr->rendering);
	return (NULL);
}
