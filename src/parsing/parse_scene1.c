/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 08:37:17 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 08:38:23 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** skip_whitespace - Skip whitespace characters in a string
** str: String to process
** Returns: Pointer to first non-whitespace character
*/
char	*skip_whitespace(char *str)
{
	if (!str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n' \
				|| *str == '\r'))
		str++;
	return (str);
}

/*
** is_empty_line - Check if line is empty or contains only whitespace
** line: Line to check
** Returns: 1 if empty, 0 otherwise
*/
int	is_empty_line(char *line)
{
	char	*trimmed;

	if (!line)
		return (1);
	trimmed = skip_whitespace(line);
	return (*trimmed == '\0' || *trimmed == '#');
}

/*
** cleanup_scene_on_error - Clean up partially parsed scene on error
** scene: Scene structure to clean up
*/
void	cleanup_scene_on_error(t_scene *scene)
{
	if (!scene)
		return ;
	cleanup_light_list(scene->lights);
	cleanup_object_list(scene->objects);
	scene->lights = NULL;
	scene->objects = NULL;
	scene->bvh_root = NULL;
}
