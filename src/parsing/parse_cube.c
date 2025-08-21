/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cube.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/19 09:05:28 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	init_cube_material(t_object *cube)
{
	cube->material.reflection = 0.0;
	cube->material.transparency = 0.0;
	cube->material.refraction_index = 1.0;
}

static void	free_cube_tokens(char *pos, char *size, char *col)
{
	safe_free((void **)&pos);
	safe_free((void **)&size);
	safe_free((void **)&col);
}

static t_object	*create_cube(t_vec3 position, double size, t_color color)
{
	t_object	*cube;

	cube = safe_malloc(sizeof(t_object));
	cube->type = CUBE;
	cube->position = position;
	cube->radius = size / 2.0;
	cube->material.color = color;
	cube->next = NULL;
	init_cube_material(cube);
	return (cube);
}

int	parse_cube(char *line, t_scene *scene)
{
	char		*tokens[3];
	t_vec3		position;
	double		size;
	t_color		color;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2])
		return (0);
	if (!parse_vec3(tokens[0], &position) || !parse_double(tokens[1], &size) \
			|| !parse_color(tokens[2], &color))
		return (free_cube_tokens(tokens[0], tokens[1], tokens[2]), 0);
	if (size <= 0)
		return (free_cube_tokens(tokens[0], tokens[1], tokens[2]), 0);
	add_object_to_scene(scene, create_cube(position, size, color));
	return (free_cube_tokens(tokens[0], tokens[1], tokens[2]), 1);
}
