/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/19 09:05:02 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_sphere_params(double diameter)
{
	return (diameter > 0);
}

static void	init_sphere_material(t_object *sphere)
{
	sphere->material.reflection = 0.3;
	sphere->material.transparency = 0.0;
	sphere->material.refraction_index = 1.5;
}

static void	free_sphere_tokens(char *pos, char *diam, char *col)
{
	free(pos);
	free(diam);
	free(col);
}

static t_object	*create_sphere(t_vec3 position, double diameter, t_color color)
{
	t_object	*sphere;

	sphere = safe_malloc(sizeof(t_object));
	sphere->type = SPHERE;
	sphere->position = position;
	sphere->radius = diameter / 2.0;
	sphere->material.color = color;
	sphere->next = NULL;
	init_sphere_material(sphere);
	return (sphere);
}

int	parse_sphere(char *line, t_scene *scene)
{
	char		*tokens[3];
	t_vec3		position;
	double		diameter;
	t_color		color;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2])
		return (0);
	if (!parse_vec3(tokens[0], &position)
		|| !parse_double(tokens[1], &diameter)
		|| !parse_color(tokens[2], &color))
		return (free_sphere_tokens(tokens[0], tokens[1], tokens[2]), 0);
	if (!validate_sphere_params(diameter))
		return (free_sphere_tokens(tokens[0], tokens[1], tokens[2]), 0);
	add_object_to_scene(scene, create_sphere(position, diameter, color));
	return (free_sphere_tokens(tokens[0], tokens[1], tokens[2]), 1);
}
