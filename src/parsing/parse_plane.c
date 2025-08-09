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

static void	init_plane_material(t_object *plane)
{
	plane->material.reflection = 0.0;
	plane->material.transparency = 0.0;
	plane->material.refraction_index = 1.0;
}

static void	free_plane_tokens(char *pos, char *norm, char *col)
{
	free(pos);
	free(norm);
	free(col);
}

static t_object	*create_plane(t_vec3 position, t_vec3 normal, t_color color)
{
	t_object	*plane;

	plane = safe_malloc(sizeof(t_object));
	plane->type = PLANE;
	plane->position = position;
	plane->normal = vec3_normalize(normal);
	plane->material.color = color;
	plane->next = NULL;
	init_plane_material(plane);
	return (plane);
}

int	parse_plane(char *line, t_scene *scene)
{
	char		*tokens[3];
	t_vec3		position;
	t_vec3		normal;
	t_color		color;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2])
		return (0);
	if (!parse_vec3(tokens[0], &position)
		|| !parse_vec3(tokens[1], &normal)
		|| !parse_color(tokens[2], &color))
		return (free_plane_tokens(tokens[0], tokens[1], tokens[2]), 0);
	if (!validate_plane_params(normal))
		return (free_plane_tokens(tokens[0], tokens[1], tokens[2]), 0);
	add_object_to_scene(scene, create_plane(position, normal, color));
	return (free_plane_tokens(tokens[0], tokens[1], tokens[2]), 1);
}