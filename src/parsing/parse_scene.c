/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/19 09:13:55 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** skip_whitespace - Skip whitespace characters in a string
** str: String to process
** Returns: Pointer to first non-whitespace character
*/
static char	*skip_whitespace(char *str)
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
static int	is_empty_line(char *line)
{
	char	*trimmed;

	if (!line)
		return (1);
	trimmed = skip_whitespace(line);
	return (*trimmed == '\0' || *trimmed == '#');
}

/*
** read_file_line - Read a single line from file
** fd: File descriptor
** Returns: Allocated string with line content, NULL on error/EOF
*/
static char	*read_file_line(int fd)
{
	int		i;
	char	*line;
	char	buffer[1024];
	int		bytes_read;

	i = 0;
	bytes_read = read(fd, &buffer[i], 1);
	while (bytes_read > 0)
	{
		if (buffer[i] == '\n' || i >= 1022)
		{
			buffer[i + 1] = '\0';
			line = safe_strdup(buffer);
			return (line);
		}
		i++;
		bytes_read = read(fd, &buffer[i], 1);
	}
	if (i > 0)
	{
		buffer[i] = '\0';
		line = safe_strdup(buffer);
		return (line);
	}
	return (NULL);
}

/*
** cleanup_scene_on_error - Clean up partially parsed scene on error
** scene: Scene structure to clean up
*/
static void	cleanup_scene_on_error(t_scene *scene)
{
	if (!scene)
		return ;
	cleanup_light_list(scene->lights);
	cleanup_object_list(scene->objects);
	scene->lights = NULL;
	scene->objects = NULL;
	scene->bvh_root = NULL;
}

/*
** validate_scene_with_filename - Validate scene with better error messages
** scene: Scene structure to validate
** filename: Filename for error reporting
** Returns: 1 on success, 0 on error
*/
static int	validate_scene_with_filename(t_scene *scene)
{
	if (!scene)
		return (printf("Error\nInvalid scene structure\n"), 0);
	if (scene->camera.fov <= 0)
		return (printf("Error\nMissing or invalid camera (C)\n"), 0); 
	if (scene->ambient_ratio < 0)
		return (printf("Error\nMissing or invalid ambient lighting (A)\n"), 0);
	if (!scene->lights)
		return (printf("Error\nMissing light source (L)\n"), 0);
	if (!scene->objects)
		return (printf("Error\nNo objects found\n"), 0);
	return (1);
}

/*
** parse_element_by_identifier - Parse element based on identifier
** identifier: Element identifier (A, C, L, sp, pl, cy, co, cu)
** rest: Rest of the line after identifier
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
static int	parse_element_by_identifier(char *identifier, char *rest, t_scene *scene)
{
	if (strcmp(identifier, "A") == 0)
		return (parse_ambient(rest, scene));
	else if (strcmp(identifier, "C") == 0)
		return (parse_camera(rest, scene));
	else if (strcmp(identifier, "L") == 0)
		return (parse_light(rest, scene));
	else if (strcmp(identifier, "sp") == 0)
		return (parse_sphere(rest, scene));
	else if (strcmp(identifier, "pl") == 0)
		return (parse_plane(rest, scene));
	else if (strcmp(identifier, "cy") == 0)
		return (parse_cylinder(rest, scene));
	else if (strcmp(identifier, "co") == 0)
		return (parse_cone(rest, scene));
	else if (strcmp(identifier, "cu") == 0)
		return (parse_cube(rest, scene));
	return (0);
}

/*
** parse_line - Parse a single line from the scene file
** line: Line to parse
** scene: Scene structure to populate
** line_num: Current line number for error reporting
** filename: Filename for error reporting
** Returns: 1 on success, 0 on error
*/
static int	parse_line(char *line, t_scene *scene, int line_num, char *filename)
{
	char	*trimmed;
	char	*identifier;
	char	*rest;
	int		result;

	if (!line || !scene)
		return (0);
	trimmed = skip_whitespace(line);
	if (is_empty_line(trimmed))
		return (1);
	rest = trimmed;
	identifier = get_next_token(&rest);
	if (!identifier)
		return (printf("Error\nInvalid syntax in %s at line %d: missing identifier\n", filename, line_num), 0); 
	result = parse_element_by_identifier(identifier, rest, scene);
	if (!result)
		printf("Error\nInvalid %s element in %s at line %d: %s\n", identifier, filename, line_num, line);
	free(identifier);
	return (result);
}

/*
** parse_scene - Main scene parsing function
** filename: Path to .rt file
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
int	parse_scene(char *filename, t_scene *scene)
{
	int		fd;
	char	*line;
	int		line_number;
	int		parse_result;

	if (!filename || !scene)
		return (printf("Error\nInvalid parameters for scene parsing\n"), 0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (printf("Error\nCannot open file: %s\n", filename), 0);
	line_number = 0;
	line = read_file_line(fd);
	while (line)
	{
		line_number++;
		parse_result = parse_line(line, scene, line_number, filename);
		if (!parse_result)
		{
			free(line);
			close(fd);
			cleanup_scene_on_error(scene);
			return (0);
		}
		free(line);
		line = read_file_line(fd);
	}
	close(fd);
	return (validate_scene_with_filename(scene));
}
