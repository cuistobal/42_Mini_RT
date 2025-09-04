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
	char		*t[3];
	t_object	*plane;

	if (!line || !scene)
		return (0);
	t[0] = get_next_token(&line);
	t[1] = get_next_token(&line);
	t[2] = get_next_token(&line);
	if (!t[0] || !t[1] || !t[2])
		return (0);
	plane = safe_malloc(sizeof(t_object));
	if (!parse_vec3(t[0], &plane->position)
		|| !parse_vec3(t[1], &plane->normal)
		|| !parse_color(t[2], &plane->material.color))
		return (free_tokens(t, 3), 0);
	if (!validate_plane_params(plane->normal))
		return (safe_free((void **)&plane), free_tokens(t, 3), 0);
	plane->centroid = plane->position;
	plane->type = PLANE;
	add_object_to_scene(scene, plane);
	return (free_tokens(t, 3), 1);
}
