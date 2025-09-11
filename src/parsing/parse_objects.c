/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/04 09:20:09 by chrleroy         ###   ########.fr       */
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

void	object_initializer(t_object *obj)
{
	obj = safe_malloc(sizeof(t_object));
	if (!obj)
		return ;
	obj->uid = -1;
	obj->type = -1;
	obj->axis = vec3_new(0, 0, 0);
	obj->normal = vec3_new(0, 0, 0);
	obj->position = vec3_new(0, 0, 0);
	obj->centroid = vec3_new(0, 0, 0);
	obj->radius = 0;
	obj->height = 0;
	obj->angle = 0;
	obj->material.color = color_new(0, 0, 0);
	obj->material.reflection = 0;
	obj->material.transparency = 0;
	obj->material.refraction_index = 1.0;
	obj->material.bump = 0;
	obj->material.chess = 0;
	obj->material.texture_addr = NULL;
	obj->next = NULL;
}