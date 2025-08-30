/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_plane_params(t_vec3 normal)
{
	return (vec3_length(normal) != 0);
}

static void	free_plane_tokens(char *pos, char *norm, char *col)
{
	safe_free((void **)&pos);
	safe_free((void **)&norm);
	safe_free((void **)&col);
}

int	parse_plane(char *line, t_scene *scene)
{
	char		*tokens[3];
	t_object	*plane;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2])
		return (0);
	plane = safe_malloc(sizeof(t_object));
	if (!parse_vec3(tokens[0], &plane->position)
		|| !parse_vec3(tokens[1], &plane->normal)
		|| !parse_color(tokens[2], &plane->material.color))
		return (free_plane_tokens(tokens[0], tokens[1], tokens[2]), 0);
	if (!validate_plane_params(plane->normal))
		return (safe_free((void **)&plane), free_plane_tokens(tokens[0], tokens[1], tokens[2]), 0);
	plane->centroid = plane->position; 
	plane->type = PLANE;
	add_object_to_scene(scene, plane);
	return (free_plane_tokens(tokens[0], tokens[1], tokens[2]), 1);
}