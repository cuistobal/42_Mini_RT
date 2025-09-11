/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/04 09:07:25 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int parse_plane_tokens(char **tokens, t_object *p, t_minirt *rt)
{
	if (!parse_vec3(tokens[0], &p->position)
		|| !parse_vec3(tokens[1], &p->normal)
		|| !parse_color(tokens[2], &p->material.color)
		|| vec3_length(p->normal) == 0)
		return (0);
	return (parse_material(rt, &p->material, tokens + PLANE_TOKEN));
}

int	parse_plane(char *line, t_minirt *rt)
{
	t_object	*plane;
	char		*t[PLANE_TOKEN + MATERIAL_TOKEN];

	if (!line || !rt)
		return (0);
	if (!get_tokens(&line, t, PLANE_TOKEN) || !get_material_tokens(&line, \
		t + PLANE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(t, PLANE_TOKEN + MATERIAL_TOKEN), 0);
	plane = safe_malloc(sizeof(t_object));
	if (!plane || !parse_plane_tokens(t, plane, rt))
		return (free_tokens(t, PLANE_TOKEN + MATERIAL_TOKEN), 0);
	plane->centroid = plane->position;
	plane->type = PLANE;
	plane->next = NULL;
	add_object_to_scene(&rt->scene, plane);
	return (free_tokens(t, PLANE_TOKEN + MATERIAL_TOKEN), 1);
}
