/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/08 12:16:59 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_cone_params(double angle, double height, t_vec3 axis)
{
	return (angle >= 0 && angle <= 90 && height > 0 && vec3_length(axis) != 0);
}

static int	parse_cone_tokens(char **tokens, t_object *c)
{
	if (!parse_vec3(tokens[0], &c->position)
		|| !parse_vec3(tokens[1], &c->axis)
		|| !parse_double(tokens[2], &c->radius)
		|| !parse_double(tokens[3], &c->height)
		|| !parse_color(tokens[4], &c->material.color))
		return (0);
	c->angle = atan(c->radius / c->height) * (180.0 / M_PI);
	return (parse_material(&c->material, tokens + CONE_TOKEN));
}

//Add the object to pass its elements by reference instead of local vartiables
int	parse_cone(char *line, t_scene *scene)
{
	int 		i;
	char		*tokens[CONE_TOKEN + MATERIAL_TOKEN];
	t_object	*cone;

	i = CONE_TOKEN + MATERIAL_TOKEN;
	if (!line || !scene)
		return (0);
	if (!get_tokens(&line, tokens, CONE_TOKEN) || !get_material_tokens(&line, \
		tokens + CONE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(tokens, i), 0);
	cone = safe_malloc(sizeof(t_object));
	if (!parse_cone_tokens(tokens, cone))
		return (safe_free((void **)&cone), free_tokens(tokens, i), 0);
	if (!validate_cone_params(cone->angle, cone->height, cone->axis))
		return (safe_free((void **)&cone), free_tokens(tokens, i), 0);
	cone->centroid = vec3_add(cone->position, vec3_mult(cone->axis, \
				cone->height * 0.25));
	cone->type = CONE;
	cone->next = NULL;
	add_object_to_scene(scene, cone);
	return (free_tokens(tokens, i), 1);
}
