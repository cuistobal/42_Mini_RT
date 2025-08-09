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
	if (diameter <= 0 || height <= 0)
		return (0);
	if (vec3_length(axis) == 0)
		return (0);
	return (1);
}

static void	init_cylinder_material(t_object *cylinder)
{
	cylinder->material.reflection = 0.0;
	cylinder->material.transparency = 0.0;
	cylinder->material.refraction_index = 1.0;
}

static void	free_cylinder_tokens(char **tokens)
{
	free(tokens[0]);
	free(tokens[1]);
	free(tokens[2]);
	free(tokens[3]);
	free(tokens[4]);
}

static void	set_cylinder_properties(t_object *cyl, t_vec3 pos, t_vec3 axis,
	double diam, double height, t_color color)
{
	cyl->type = CYLINDER;
	cyl->position = pos;
	cyl->axis = vec3_normalize(axis);
	cyl->radius = diam / 2.0;
	cyl->height = height;
	cyl->material.color = color;
	cyl->next = NULL;
	init_cylinder_material(cyl);
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

static int	create_and_add_cylinder(t_scene *scene, t_vec3 pos, t_vec3 axis,
	double diam, double height, t_color color)
{
	t_object	*cylinder;

	cylinder = safe_malloc(sizeof(t_object));
	set_cylinder_properties(cylinder, pos, axis, diam, height, color);
	add_object_to_scene(scene, cylinder);
	return (1);
}

int	parse_cylinder(char *line, t_scene *scene)
{
	char		*tokens[5];
	t_vec3		position;
	t_vec3		axis;
	double		diameter;
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
		return (free_cylinder_tokens(tokens), 0);
	if (!parse_cylinder_tokens(tokens, &position, &axis, &diameter, &height,
		&color))
		return (free_cylinder_tokens(tokens), 0);
	if (!validate_cylinder_params(diameter, height, axis))
		return (free_cylinder_tokens(tokens), 0);
	free_cylinder_tokens(tokens);
	return (create_and_add_cylinder(scene, position, axis, diameter, height,
		color));
}