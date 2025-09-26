/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/26 08:18:47 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_c_params(double diameter, double height, \
		t_vec3 axis)
{
	return (diameter > 0 && height > 0 && vec3_length(axis) != 0);
}	

static int	parse_c_tokens(char **tokens, t_object *c, t_minirt *rt)
{
	if (!parse_vec3(tokens[0], &c->position)
		|| !parse_vec3(tokens[1], &c->axis)
		|| !parse_double(tokens[2], &c->radius)
		|| !parse_double(tokens[3], &c->height)
		|| !parse_color(tokens[4], &c->material.color))
		return (0);
	return (parse_material(rt, &c->material, tokens + CYLINDER_TOKEN));
}

int	parse_cylinder(char *line, t_minirt *rt)
{
	int			i;
	char		*tokens[CYLINDER_TOKEN + MATERIAL_TOKEN];
	t_object	*c;
	t_vec3		end;

	i = CYLINDER_TOKEN + MATERIAL_TOKEN;
	if (!line || !rt)
		return (0);
	if (!get_tokens(&line, tokens, CYLINDER_TOKEN) || !get_material_tokens(
			&line, tokens + CYLINDER_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(tokens, i), 0);
	c = safe_malloc(sizeof(t_object));
	if (!c)
		return (free_tokens(tokens, i), 0);
	if (!parse_c_tokens(tokens, c, rt))
		return (error_helper(c, tokens, i));
	if (!validate_c_params(c->radius / 2.0, \
				c->height, c->axis))
		return (error_helper(c, tokens, i));
	end = vec3_add(c->position, \
			vec3_mult(c->axis, c->height));
	c->centroid = vec3_mult(vec3_add(c->position, end), 0.5);
	c->type = CYLINDER;
	c->next = NULL;
	return (free_tokens(tokens, i), add_object_to_scene(&rt->scene, c), 1);
}
