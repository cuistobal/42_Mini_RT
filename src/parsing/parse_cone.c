/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/22 12:51:45 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"


static int	validate_cone_params(double angle, double height, t_vec3 axis)
{
	return (angle > 0 && angle < 90 && height > 0 && vec3_length(axis) != 0);
}

static void	free_cone_tokens(char **tokens)
{
	safe_free((void **)&tokens[0]);
	safe_free((void **)&tokens[1]);
	safe_free((void **)&tokens[2]);
	safe_free((void **)&tokens[3]);
	safe_free((void **)&tokens[4]);
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
	t_object	*cone;

	if (!line || !scene)
		return (0);
	tokens[0] = get_next_token(&line);
	tokens[1] = get_next_token(&line);
	tokens[2] = get_next_token(&line);
	tokens[3] = get_next_token(&line);
	tokens[4] = get_next_token(&line);
	if (!tokens[0] || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4])
		return (free_cone_tokens(tokens), 0);
	cone = safe_malloc(sizeof(t_object));
	if (!parse_cone_tokens(tokens, &cone->position, &cone->axis, &cone->angle, &cone->height, &cone->material.color))
		return (safe_free((void **)&cone), free_cone_tokens(tokens), 0);
	if (!validate_cone_params(cone->angle, cone->height, cone->axis))
		return (safe_free((void **)&cone), free_cone_tokens(tokens), 0);
	cone->centroid = vec3_add(cone->position, vec3_mult(cone->axis, \
				cone->height * 0.25));
	cone->type = CONE;
	add_object_to_scene(scene, cone);
	return (free_cone_tokens(tokens), 1);
}
