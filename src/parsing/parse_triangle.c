/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/19 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int is_degenerate_triangle(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
    return (vec3_length(vec3_cross(vec3_sub(v1, v0), vec3_sub(v2, v0))) < EPSILON);
}

static inline void	free_triangle_tokens(char *t0, char *t1, char *t2, char *t3)
{
	safe_free((void **)&t0);
	safe_free((void **)&t1);
	safe_free((void **)&t2);
	safe_free((void **)&t3);
}

static inline t_vec3 compute_centroid(t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
    return (vec3_new(
        (v0.x + v1.x + v2.x) / 3.0,
        (v0.y + v1.y + v2.y) / 3.0,
        (v0.z + v1.z + v2.z) / 3.0
    ));
}

int parse_triangle(char *line, t_scene *scene)
{
    char    *tokens[4];
    t_object *triangle;

    if (!line || !scene)
        return (0);
    tokens[0] = get_next_token(&line);
    tokens[1] = get_next_token(&line);
    tokens[2] = get_next_token(&line);
    tokens[3] = get_next_token(&line);
    if (!tokens[0] || !tokens[1] || !tokens[2] || !tokens[3])
        return (0);
    triangle = safe_malloc(sizeof(t_object));
    if (!triangle)
        return (free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]), 0);
    if (!parse_vec3(tokens[0], &triangle->position)
        || !parse_vec3(tokens[1], &triangle->normal)
        || !parse_vec3(tokens[2], &triangle->axis)
        || !parse_color(tokens[3], &triangle->material.color))
        return (safe_free((void **)&triangle), free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]), 0);
    if (is_degenerate_triangle(triangle->position, triangle->normal, triangle->axis))
        return (safe_free((void **)&triangle), free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]), 0);
    triangle->centroid = compute_centroid(triangle->position, triangle->normal, triangle->axis);
    triangle->type = TRIANGLE;
    add_object_to_scene(scene, triangle);
    return (free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]), 1);
}