/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/04 09:08:11 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	validate_sphere_params(double diameter)
{
	return (diameter > 0);
}

int	parse_sphere(char *line, t_scene *scene)
{
	char		*tokens[SPHERE_TOKEN + MATERIAL_TOKEN];
	t_object	*sphere;

	if (!line || !scene)
		return (0);
	if (!get_tokens(&line, tokens, SPHERE_TOKEN) || !get_material_tokens(&line, \
		tokens + SPHERE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(tokens, SPHERE_TOKEN + MATERIAL_TOKEN), 0);
	sphere = safe_malloc(sizeof(t_object));
	if (!parse_vec3(tokens[0], &sphere->position)
		|| !parse_double(tokens[1], &sphere->radius)
		|| !parse_color(tokens[2], &sphere->material.color))
		return (error_helper(sphere, tokens, SPHERE_TOKEN + MATERIAL_TOKEN));
	if (!parse_material(&sphere->material, tokens + SPHERE_TOKEN))
		return (error_helper(sphere, tokens, SPHERE_TOKEN + MATERIAL_TOKEN));
	if (!validate_sphere_params(sphere->radius / 2.0))
		return (error_helper(sphere, tokens, SPHERE_TOKEN + MATERIAL_TOKEN));
	sphere->centroid = sphere->position;
	sphere->type = SPHERE;
	sphere->next = NULL;
	add_object_to_scene(scene, sphere);
	return (free_tokens(tokens, SPHERE_TOKEN + MATERIAL_TOKEN), 1);
}
