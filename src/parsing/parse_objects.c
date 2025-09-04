/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 09:12:17 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

// Worst way of inserting a new element to a linked list
void	add_object_to_scene(t_scene *scene, t_object *object)
{
	t_object	*current;

	if (!scene || !object)
		return ;
	if (!scene->objects)
	{
		scene->objects = object;
		return ;
	}
	current = scene->objects;
	while (current->next)
		current = current->next;
	current->next = object;
}
