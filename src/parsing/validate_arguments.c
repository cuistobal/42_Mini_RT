/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:54:39 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/09 12:57:05 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	validate_file_extension(char *filename)
{
	int		len;
	char	*ext;

	len = strlen(filename);
	if (len < 4)
	{
		printf("Error\nInvalid file extension. Expected .rt file\n");
		return (0);
	}
	ext = filename + len - 3;
	if (strcmp(ext, ".rt") != 0)
	{
		printf("Error\nInvalid file extension. Expected .rt file\n");
		return (0);
	}
	return (1);
}

int	validate_file_access(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("Error\nCannot open file: %s\n", filename);
		return (0);
	}
	return (close(fd), 1);
}

int	validate_arguments(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Error\nUsage: %s <scene_file.rt>\n", argv[0]);
		return (0);
	}
	if (!argv[1] || argv[1][0] == '\0')
	{
		printf("Error\nEmpty filename provided\n");
		return (0);
	}
	return (validate_file_extension(argv[1]) && \
		validate_file_access(argv[1]));
}