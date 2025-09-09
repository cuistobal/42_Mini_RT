/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cube.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 09:05:04 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static t_object	*create_cube(t_vec3 position, double size, t_color color)
{
	t_object	*cube;

	cube = safe_malloc(sizeof(t_object));
	cube->type = CUBE;
	cube->position = position;
	cube->radius = size / 2.0;
	cube->material.color = color;
	cube->next = NULL;
	return (cube);
}

int	parse_cube(char *line, t_minirt *rt)
{
	char		*tokens[CUBE_TOKEN + MATERIAL_TOKEN];
	t_vec3		position;
	double		size;
	t_color		color;
	t_object	*cube;

	if (!line || !rt) 
		return (0);
	if (!get_tokens(&line, tokens, CUBE_TOKEN) || !get_material_tokens(&line, \
		tokens + CUBE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(tokens, CUBE_TOKEN + MATERIAL_TOKEN), 0);
	if (!parse_vec3(tokens[0], &position) || !parse_double(tokens[1], &size) \
			|| !parse_color(tokens[2], &color))
		return (free_tokens(tokens, CUBE_TOKEN + MATERIAL_TOKEN), 0);
	if (size <= 0)
		return (free_tokens(tokens, CUBE_TOKEN + MATERIAL_TOKEN), 0);
	cube = create_cube(position, size, color);
	cube->centroid = vec3_new(position.x + size / 2.0, position.y + size \
			/ 2.0, position.z + size / 2.0);
	cube->type = CUBE;
	cube->next = NULL;
	parse_material(rt, &cube->material, tokens + CUBE_TOKEN);
	return (add_object_to_scene(&rt->scene, cube), \
		free_tokens(tokens, CUBE_TOKEN + MATERIAL_TOKEN), 1);
}
