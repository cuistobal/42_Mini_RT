/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/26 08:04:44 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	parse_sphere_tokens(char **tokens, t_object *s, t_minirt *rt)
{
	if (!parse_vec3(tokens[0], &s->position)
		|| !parse_double(tokens[1], &s->radius)
		|| !parse_color(tokens[2], &s->material.color)
		|| s->radius <= 0)
		return (0);
	return (parse_material(rt, &s->material, tokens + SPHERE_TOKEN));
}

int	parse_sphere(char *line, t_minirt *rt)
{
	char		*tokens[SPHERE_TOKEN + MATERIAL_TOKEN];
	t_object	*sphere;

	if (!line || !rt)
		return (0);
	if (!get_tokens(&line, tokens, SPHERE_TOKEN) || !get_material_tokens(&line, \
		tokens + SPHERE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(tokens, SPHERE_TOKEN + MATERIAL_TOKEN), 0);
	sphere = safe_malloc(sizeof(t_object));
	if (!sphere || !parse_sphere_tokens(tokens, sphere, rt))
		return (error_helper(sphere, tokens, SPHERE_TOKEN + MATERIAL_TOKEN));
	sphere->centroid = sphere->position;
	sphere->type = SPHERE;
	sphere->next = NULL;
	add_object_to_scene(&rt->scene, sphere);
	return (free_tokens(tokens, SPHERE_TOKEN + MATERIAL_TOKEN), 1);
}
