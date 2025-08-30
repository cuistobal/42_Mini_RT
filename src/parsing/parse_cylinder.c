/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_cylinder_params(double diameter, double height, t_vec3 axis)
{
	return (diameter > 0 && height > 0 && vec3_length(axis) != 0);
}	

static void	free_cylinder_tokens(char **tokens)
{
	safe_free((void **)&tokens[0]);
	safe_free((void **)&tokens[1]);
	safe_free((void **)&tokens[2]);
	safe_free((void **)&tokens[3]);
	safe_free((void **)&tokens[4]);
}

static int	parse_cylinder_tokens(char **tokens, t_vec3 *pos, t_vec3 *axis,
	double *diam, double *height, t_color *color)
{
	if (!parse_vec3(tokens[0], pos)
		|| !parse_vec3(tokens[1], axis)
		|| !parse_double(tokens[2], diam)
		|| !parse_double(tokens[3], height)
		|| !parse_color(tokens[4], color))
		return (0);
	return (1);
}

int	parse_cylinder(char *line, t_scene *scene)
{
	char		*tokens[5];
	t_object	*cylinder;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	tokens[3] = get_next_token(&line);
	tokens[4] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
		return (free_cylinder_tokens(tokens), 0);
	cylinder = safe_malloc(sizeof(t_object));
	if (!parse_cylinder_tokens(tokens, &cylinder->position, &cylinder->axis,
		&cylinder->radius, &cylinder->height, &cylinder->material.color))
		return (safe_free((void **)&cylinder), free_cylinder_tokens(tokens), 0);
	if (!validate_cylinder_params(cylinder->radius / 2.0, cylinder->height, cylinder->axis))
		return (safe_free((void **)&cylinder), free_cylinder_tokens(tokens), 0);
	t_vec3 end_point = vec3_add(cylinder->position, vec3_mult(cylinder->axis, cylinder->height));
	cylinder->centroid = vec3_mult(vec3_add(cylinder->position, end_point), 0.5);
	cylinder->type = CYLINDER;
	free_cylinder_tokens(tokens);
	return (add_object_to_scene(scene, cylinder), 1);
}