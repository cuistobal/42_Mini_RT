/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/04 08:40:41 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	is_degenerate_tr(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
	return (vec3_length(vec3_cross(vec3_sub(v1, v0), \
					vec3_sub(v2, v0))) < EPSILON);
}

static inline t_vec3	compute_centroid(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
	return (vec3_new((v0.x + v1.x + v2.x) / 3.0, (v0.y + v1.y + v2.y) / 3.0, \
				(v0.z + v1.z + v2.z) / 3.0));
}

int parse_triangle_tokens(char **tokens, t_object *tr)
{
	if (!parse_vec3(tokens[0], &tr->position)
		|| !parse_vec3(tokens[1], &tr->normal)
		|| !parse_vec3(tokens[2], &tr->axis)
		|| !parse_color(tokens[3], &tr->material.color)
		|| is_degenerate_tr(tr->position, tr->normal, tr->axis))
		return (0);
	return (parse_material(&tr->material, tokens + TRIANGLE_TOKEN));
}

// 4 tokens + 6 material tokens
int	parse_triangle(char *line, t_scene *scene)
{
	int			i;
	t_object	*tr;
	char		*t[TRIANGLE_TOKEN + MATERIAL_TOKEN];

	i = TRIANGLE_TOKEN + MATERIAL_TOKEN;
	if (!line || !scene)
		return (0);
	if (!get_tokens(&line, t, TRIANGLE_TOKEN) || !get_material_tokens(&line, \
		t + TRIANGLE_TOKEN, MATERIAL_TOKEN))
		return (free_tokens(t, i), 0);
	tr = safe_malloc(sizeof(t_object));
	if (!tr || !parse_triangle_tokens(t, tr))
		return (error_helper(tr, t, i));
	tr->centroid = compute_centroid(tr->position, tr->normal, tr->axis);
	tr->type = TRIANGLE;
	tr->next = NULL;
	return (add_object_to_scene(scene, tr), free_tokens(t, i), 1);
}