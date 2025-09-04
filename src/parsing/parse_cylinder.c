/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 08:29:44 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_cylinder_params(double diameter, double height, \
		t_vec3 axis)
{
	return (diameter > 0 && height > 0 && vec3_length(axis) != 0);
}	

static int	parse_cylinder_tokens(char **tokens, t_object *c)
{
	if (!parse_vec3(tokens[0], &c->position)
		|| !parse_vec3(tokens[1], &c->axis)
		|| !parse_double(tokens[2], &c->radius)
		|| !parse_double(tokens[3], &c->height)
		|| !parse_color(tokens[4], &c->material.color))
		return (0);
	return (1);
}

int	parse_cylinder(char *line, t_scene *scene)
{
	char		*tokens[5];
	t_object	*cylinder;
	t_vec3		end;

	if (!line || !scene)
		return (0);
	if (!get_tokens(&line, tokens, 5))
		return (free_tokens(tokens, 5), 0);
	if (!tokens[0] || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
		return (free_tokens(tokens, 5), 0);
	cylinder = safe_malloc(sizeof(t_object));
	if (!parse_cylinder_tokens(tokens, cylinder))
		return (safe_free((void **)&cylinder), free_tokens(tokens, 5), 0);
	if (!validate_cylinder_params(cylinder->radius / 2.0, \
				cylinder->height, cylinder->axis))
		return (safe_free((void **)&cylinder), free_tokens(tokens, 5), 0);
	end = vec3_add(cylinder->position, \
			vec3_mult(cylinder->axis, cylinder->height));
	cylinder->centroid = vec3_mult(vec3_add(cylinder->position, end), 0.5);
	cylinder->type = CYLINDER;
	free_tokens(tokens, 5);
	return (add_object_to_scene(scene, cylinder), 1);
}
