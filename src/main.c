/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 09:38:34 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	validate_file_extension(char *filename)
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

static int	validate_file_access(char *filename)
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

static int	validate_arguments(int argc, char **argv)
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
	if (!validate_file_extension(argv[1]))
		return (0);
	if (!validate_file_access(argv[1]))
		return (0);
	return (1);
}

static void	init_scene(t_scene *scene)
{
	scene->lights = NULL;
	scene->objects = NULL;
	scene->bvh_root = NULL;
	scene->ambient_ratio = -1.0;
	scene->camera.fov = 0.0;
	scene->ambient = color_new(0, 0, 0);
}

static void	init_mlx_context(t_mlx *mlx)
{
	mlx->mlx_ptr = NULL;
	mlx->win_ptr = NULL;
	mlx->img_ptr = NULL;
	mlx->img_data = NULL;
	mlx->width = 0;
	mlx->height = 0;
	mlx->bits_per_pixel = 0;
	mlx->size_line = 0;
	mlx->endian = 0;
}

static int	init_minirt(t_minirt *rt, char *filename)
{
	if (!rt || !filename)
		return (0);
	rt->filename = filename;
	init_scene(&rt->scene);
	init_mlx_context(&rt->mlx);
	return (1);
}

int	main(int argc, char **argv)
{
	t_minirt	rt;

	if (!validate_arguments(argc, argv))
		return (ERROR_ARGS);
	if (!init_minirt(&rt, argv[1]))
		return (ERROR_MEMORY);
	if (!parse_scene(rt.filename, &rt.scene))
	{
		printf("Error\nFailed to parse scene file\n");
		cleanup_all(&rt);
		return (ERROR_PARSE);
	}
	rt.scene.bvh_root = build_bvh(&rt.scene);
	if (init_mlx(&rt.mlx) != 0)
	{
		printf("Error\nFailed to initialize graphics\n");
		cleanup_all(&rt);
		return (ERROR_MLX);
	}
	setup_hooks(&rt);
	mlx_loop(rt.mlx.mlx_ptr);
	return (cleanup_all(&rt), ERROR_NONE);
}
