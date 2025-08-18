/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_bvh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:50:09 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 08:53:31 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** count_objects - Count objects in linked list
** cringe time complexity, need to modify the build_bvh function
*/
static inline int	count_objects(t_object *objects)
{
	int	count;

	count = 0;
	while (objects)
	{
		count++;
		objects = objects->next;
	}
	return (count);
}

/*
** build_bvh - Build BVH from scene objects
*/
t_bvh_node	*build_bvh(t_scene *scene)
{
	int			i;
	int			count;
	t_bvh_node	*root;
	t_object	*current;
	t_object	**object_array;

	if (!scene || !scene->objects)
		return (NULL);
	count = count_objects(scene->objects);
	if (count == 0)
		return (NULL);
	object_array = safe_malloc(sizeof(t_object *) * count);
	if (!object_array)
		return (NULL);
	i = 0;
	current = scene->objects;
	while (current && i < count)
	{
		object_array[i] = current;
		current = current->next;
		i++;
	}
	root = build_bvh_recursive(object_array, count);
	return (safe_free((void **)&object_array), root);
}
