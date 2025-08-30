/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 09:04:04 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	is_degenerate_tr(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
	return (vec3_length(vec3_cross(vec3_sub(v1, v0), \
					vec3_sub(v2, v0))) < EPSILON);
}

static inline void	free_tr_t(char *t0, char *t1, char *t2, char *t3)
{
	safe_free((void **)&t0);
	safe_free((void **)&t1);
	safe_free((void **)&t2);
	safe_free((void **)&t3);
}

static inline t_vec3	compute_centroid(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
	return (vec3_new((v0.x + v1.x + v2.x) / 3.0, (v0.y + v1.y + v2.y) / 3.0, \
				(v0.z + v1.z + v2.z) / 3.0));
}

int	parse_tr(char *line, t_scene *scene)
{
	char		*t[4];
	t_object	*tr;

	if (!line || !scene)
		return (0);
	t[0] = get_next_token(&line);
	t[1] = get_next_token(&line);
	t[2] = get_next_token(&line);
	t[3] = get_next_token(&line);
	if (!t[0] || !t[1] || !t[2] || !t[3])
		return (0);
	tr = safe_malloc(sizeof(t_object));
	if (!tr)
		return (free_tr_t(t[0], t[1], t[2], t[3]), 0);
	if (!parse_vec3(t[0], &tr->position)
		|| !parse_vec3(t[1], &tr->normal)
		|| !parse_vec3(t[2], &tr->axis)
		|| !parse_color(t[3], &tr->material.color))
		return (safe_free((void **)&tr), free_tr_t(t[0], t[1], t[2], t[3]), 0);
	if (is_degenerate_tr(tr->position, tr->normal, tr->axis))
		return (safe_free((void **)&tr), free_tr_t(t[0], t[1], t[2], t[3]), 0);
	tr->centroid = compute_centroid(tr->position, tr->normal, tr->axis);
	tr->type = TRIANGLE;
	add_object_to_scene(scene, tr);
	return (free_tr_t(t[0], t[1], t[2], t[3]), 1);
}
