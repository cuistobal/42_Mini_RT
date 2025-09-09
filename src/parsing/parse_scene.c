/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/04 07:30:57 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

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
static int	parse_element_by_identifier(char *identifier, char *rest, \
		t_minirt *rt)
{
	if (strcmp(identifier, "A") == 0)
		return (parse_ambient(rest, rt));
	else if (strcmp(identifier, "C") == 0)
		return (parse_camera(rest, rt));
	else if (strcmp(identifier, "L") == 0)
		return (parse_light(rest, rt));
	else if (strcmp(identifier, "sp") == 0)
		return (parse_sphere(rest, rt));
	else if (strcmp(identifier, "pl") == 0)
		return (parse_plane(rest, rt));
	else if (strcmp(identifier, "cy") == 0)
		return (parse_cylinder(rest, rt));
	else if (strcmp(identifier, "co") == 0)
		return (parse_cone(rest, rt));
	else if (strcmp(identifier, "cu") == 0)
		return (parse_cube(rest, rt));
	else if (strcmp(identifier, "tr") == 0)
		return (parse_triangle(rest, rt));
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
static int	parse_line(t_minirt *rt, char *line, int line_num)
{
	int		res;
	char	*rest;
	char	*trimmed;
	char	*identifier;

	if (!line || !rt)
		return (0);
	trimmed = skip_whitespace(line);
	if (is_empty_line(trimmed))
		return (1);
	rest = trimmed;
	identifier = get_next_token(&rest);
	if (!identifier)
		return (printf("Error\nInvalid syntax in %s at line %d: \
					missing identifier\n", rt->filename, line_num), 0);
	res = parse_element_by_identifier(identifier, rest, rt);
	if (!res)
		printf("Error\nInvalid %s element in %s at line %d: \
				%s\n", identifier, rt->filename, line_num, line);
	return (safe_free((void **)&identifier), res);
}

/*
** parse_scene - Main scene parsing function
** filename: Path to .rt file
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
int	parse_scene(t_minirt *rt)
{
	int		fd;
	char	*line;
	int		line_number;
	int		parse_result;

	if (!rt || !rt->filename)
		return (printf("Error\nInvalid parameters for scene parsing\n"), 0);
	fd = open(rt->filename, O_RDONLY);
	if (fd < 0)
		return (printf("Error\nCannot open file: %s\n", rt->filename), 0);
	line_number = 0;
	line = read_file_line(fd);
	while (line)
	{
		line_number++;
		parse_result = parse_line(rt, line, line_number);
		if (!parse_result)
			return (safe_free((void **)&line), close(fd), \
					cleanup_scene_on_error(&rt->scene), 0);
		safe_free((void **)&line);
		line = read_file_line(fd);
	}
	return (close(fd), validate_scene_with_filename(&rt->scene));
}
