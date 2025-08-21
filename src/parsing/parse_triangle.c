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

static t_object *create_triangle(t_vec3 v0, t_vec3 v1, t_vec3 v2, t_color color)
{
    t_object *tri;

    tri = safe_malloc(sizeof(t_object));
    tri->type = TRIANGLE;
    tri->position = v0;
    tri->normal = v1;
    tri->axis = v2;
    tri->material.color = color;
    tri->next = NULL;
    return (tri);
}

int parse_triangle(char *line, t_scene *scene)
{
    char    *tokens[4];
    t_vec3  v0;
    t_vec3  v1;
    t_vec3  v2;
    t_color color;

    if (!line || !scene)
        return (0);
    tokens[0] = get_next_token(&line);
    tokens[1] = get_next_token(&line);
    tokens[2] = get_next_token(&line);
    tokens[3] = get_next_token(&line);
    if (!tokens[0] || !tokens[1] || !tokens[2] || !tokens[3])
        return (0);
    if (!parse_vec3(tokens[0], &v0)
        || !parse_vec3(tokens[1], &v1)
        || !parse_vec3(tokens[2], &v2)
        || !parse_color(tokens[3], &color))
        return (free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]), 0);
    if (is_degenerate_triangle(v0, v1, v2))
        return (free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]), 0);
    add_object_to_scene(scene, create_triangle(v0, v1, v2, color));
    free_triangle_tokens(tokens[0], tokens[1], tokens[2], tokens[3]);
    return (1);
}