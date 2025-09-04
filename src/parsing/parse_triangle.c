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

int	parse_triangle(char *line, t_scene *scene)
{
	// 4 tokens + 6 material tokens
	char		*t[10];
	t_object	*tr;

	if (!line || !scene)
		return (0);
	if (!get_tokens(&line, t, 4))
		return (free_tokens(t, 4), 0);
	if (!get_material_tokens(&line, t + 4, 6))
		return (free_tokens(t, 10), 0);
	tr = safe_malloc(sizeof(t_object));
	if (!tr)
		return (free_tokens(t, 10), 0);
	if (!parse_vec3(t[0], &tr->position)
		|| !parse_vec3(t[1], &tr->normal)
		|| !parse_vec3(t[2], &tr->axis)
		|| !parse_color(t[3], &tr->material.color))
		return (safe_free((void **)&tr), free_tokens(t, 10), 0);
	if (is_degenerate_tr(tr->position, tr->normal, tr->axis))
		return (safe_free((void **)&tr), free_tokens(t, 10), 0);
	if (!parse_material(&tr->material, t + 4))
		return (safe_free((void **)&tr), free_tokens(t, 10), 0);
	tr->centroid = compute_centroid(tr->position, tr->normal, tr->axis);
	tr->type = TRIANGLE;
	add_object_to_scene(scene, tr);
	return (free_tokens(t, 10), 1);
}
