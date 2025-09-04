/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 09:12:40 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_plane_params(t_vec3 normal)
{
	return (vec3_length(normal) != 0);
}

int	parse_plane(char *line, t_scene *scene)
{
	char		*t[PLANE_TOKEN + MATERIAL_TOKEN];
	t_object	*plane;

	if (!line || !scene)
		return (0);
	if (!get_tokens(&line, t, PLANE_TOKEN) || !get_material_tokens(&line, \
		t + PLANE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(t, PLANE_TOKEN + MATERIAL_TOKEN), 0);
	plane = safe_malloc(sizeof(t_object));
	if (!parse_vec3(t[0], &plane->position)
		|| !parse_vec3(t[1], &plane->normal)
		|| !parse_color(t[2], &plane->material.color))
		return (free_tokens(t, PLANE_TOKEN + MATERIAL_TOKEN), 0);
	if (!validate_plane_params(plane->normal))
		return (safe_free((void **)&plane), free_tokens(t, \
			PLANE_TOKEN + MATERIAL_TOKEN), 0);
	plane->centroid = plane->position;
	plane->type = PLANE;
	add_object_to_scene(scene, plane);
	return (free_tokens(t, PLANE_TOKEN + MATERIAL_TOKEN), 1);
}
