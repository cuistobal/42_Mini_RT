/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** safe_malloc - Allocate memory with error checking
** size: Size to allocate
** Returns: Allocated memory or exits on failure
*/
void	*safe_malloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
	{
		printf("Error\nMemory allocation failed\n");
		exit(1);
	}
	return (ptr);
}

/*
** safe_strdup - Duplicate string with error checking
** str: String to duplicate
** Returns: Duplicated string or exits on failure
*/
char	*safe_strdup(char *str)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!str)
		return (NULL);
	len = strlen(str);
	dup = safe_malloc(len + 1);
	i = 0;
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*
** safe_free - Free memory safely (sets pointer to NULL)
** ptr: Pointer to memory to free
*/
void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}/*

** safe_calloc - Allocate and zero-initialize memory with error checking
** count: Number of elements
** size: Size of each element
** Returns: Allocated and zeroed memory or exits on failure
*/
void	*safe_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total_size;

	if (count == 0 || size == 0)
		return (NULL);
	
	// Check for overflow
	if (count > SIZE_MAX / size)
	{
		printf("Error\nMemory allocation overflow\n");
		exit(1);
	}
	
	total_size = count * size;
	ptr = safe_malloc(total_size);
	if (ptr)
		memset(ptr, 0, total_size);
	
	return (ptr);
}

/*
** safe_realloc - Reallocate memory with error checking
** ptr: Pointer to existing memory (can be NULL)
** size: New size
** Returns: Reallocated memory or exits on failure
*/
void	*safe_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (size == 0)
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	
	new_ptr = realloc(ptr, size);
	if (!new_ptr)
	{
		printf("Error\nMemory reallocation failed\n");
		exit(1);
	}
	
	return (new_ptr);
}

/*
** cleanup_string_array - Free array of strings
** array: Array of string pointers
** count: Number of strings in array
*/
void	cleanup_string_array(char **array, int count)
{
	int	i;

	if (!array)
		return ;
	
	i = 0;
	while (i < count && array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

/*
** cleanup_light_list - Free linked list of lights
** lights: Pointer to first light in list
*/
void	cleanup_light_list(t_light *lights)
{
	t_light	*current;
	t_light	*next;

	current = lights;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

/*
** cleanup_object_list - Free linked list of objects
** objects: Pointer to first object in list
*/
void	cleanup_object_list(t_object *objects)
{
	t_object	*current;
	t_object	*next;

	current = objects;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}