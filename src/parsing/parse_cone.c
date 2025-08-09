/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 09:53:28 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_cone_params(double angle, double height, t_vec3 axis)
{
	return (angle > 0 && angle < 90 && height > 0 && vec3_length(axis) != 0);
}

static void	init_cone_material(t_object *cone)
{
	cone->material.reflection = 0.0;
	cone->material.transparency = 0.0;
	cone->material.refraction_index = 1.0;
}

static void	free_cone_tokens(char **tokens)
{
	free(tokens[0]);
	free(tokens[1]);
	free(tokens[2]);
	free(tokens[3]);
	free(tokens[4]);
}

static t_object	*create_cone(t_vec3 pos, t_vec3 axis, double angle,
	double height, t_color color)
{
	t_object	*cone;

	cone = safe_malloc(sizeof(t_object));
	cone->type = CONE;
	cone->position = pos;
	cone->axis = vec3_normalize(axis);
	cone->angle = angle;
	cone->height = height;
	cone->material.color = color;
	cone->next = NULL;
	init_cone_material(cone);
	return (cone);
}

static int	parse_cone_tokens(char **tokens, t_vec3 *pos, t_vec3 *axis,
	double *angle, double *height, t_color *color)
{
	if (!parse_vec3(tokens[0], pos) ||
		!parse_vec3(tokens[1], axis) ||
		!parse_double(tokens[2], angle) ||
		!parse_double(tokens[3], height) ||
		!parse_color(tokens[4], color))
		return (0);
	return (1);
}

//Add the object to pass its elements by reference instead of local vartiables
int	parse_cone(char *line, t_scene *scene)
{
	char		*tokens[5];
	t_vec3		position;
	t_vec3		axis;
	double		angle;
	double		height;
	t_color		color;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	tokens[3] = get_next_token(&line);
	tokens[4] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
		return (free_cone_tokens(tokens), 0);
	if (!parse_cone_tokens(tokens, &position, &axis, &angle, &height, &color))
		return (free_cone_tokens(tokens), 0);
	if (!validate_cone_params(angle, height, axis))
		return (free_cone_tokens(tokens), 0);
	add_object_to_scene(scene, create_cone(position, axis, angle, height,
		color));
	return (free_cone_tokens(tokens), 1);
}
