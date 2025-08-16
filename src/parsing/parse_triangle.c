/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:14:49 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/16 08:10:45 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


static int	validate_triangle_params(t_vec3 normal)
{
	return (vec3_length(normal) != 0);
}

static void	init_plane_material(t_object *plane)
{
	plane->material.reflection = 0.0;
	plane->material.transparency = 0.0;
	plane->material.refraction_index = 1.0;
}

static void	free_triangle_tokens(char *pos, char *norm, char *col)
{
	free(pos);
	free(norm);
	free(col);
}

static t_object	*create_triangle(t_vec3 position, t_vec3 normal, t_color color)
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


int	parse_triangle(char *line, t_scene *scene)
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
		return (free_triangle_tokens(tokens[0], tokens[1], tokens[2]), 0);
	if (!validate_triangle_params(normal))
		return (free_triangle_tokens(tokens[0], tokens[1], tokens[2]), 0);
	add_object_to_scene(scene, create_triangle(position, normal, color));
	return (free_triangle_tokens(tokens[0], tokens[1], tokens[2]), 1);
}
