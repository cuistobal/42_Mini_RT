/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:25:51 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 08:48:58 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

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
		safe_free((void **)&current);	
		current = next;
	}
}

/*
** cleanup_bvh - Free BVH tree memory
*/
void	cleanup_bvh(t_bvh_node *node)
{
	if (!node)
		return ;
	cleanup_bvh(node->left);
	cleanup_bvh(node->right);
	if (node->objects)
		safe_free((void **)&node->objects);
	safe_free((void **)&node);
}
