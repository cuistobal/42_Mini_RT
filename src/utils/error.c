/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/12 09:37:19 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** handle_parse_error - Handle parsing errors with line information
** filename: Name of file being parsed
** line_num: Line number where error occurred
** message: Specific error message
** rt: Main program structure for cleanup
*/
void	handle_parse_error(char *filename, int line_num, char *message, \
		t_minirt *rt)
{
	if (!filename)
		filename = "unknown file";
	if (!message)
		message = "syntax error";
	printf("Error\nParsing error in %s at line %d: %s\n", filename, line_num, \
			message);
	if (rt)
		cleanup_all(rt);
	exit(1);
}

void	cleanup_scene(t_mlx mlx, t_scene *scene)
{
	if (!scene)
		return ;
	cleanup_light_list(scene->lights);
	cleanup_object_list(mlx, scene->objects);
	cleanup_bvh(scene->bvh_root);
	scene->lights = NULL;
	scene->objects = NULL;
	scene->bvh_root = NULL;
}

void	cleanup_all(t_minirt *rt)
{
	if (!rt)
		return ;
	cleanup_scene(rt->mlx, &rt->scene);
	cleanup_mlx(&rt->mlx);
}

int	validate_scene(t_scene *scene)
{
	if (!scene)
		return (printf("Error\nInvalid scene structure\n"), 0);
	if (scene->camera.fov <= 0)
	{
		printf("Error\nScene must contain exactly one camera (C)\n");
		return (0);
	}
	if (scene->ambient_ratio < 0)
	{
		printf("Error\nScene must contain exactly one ambient lighting (A)\n");
		return (0);
	}
	if (!scene->lights)
	{
		printf("Error\nScene must contain at least one light (L)\n");
		return (0);
	}
	if (!scene->objects)
	{
		printf("Error\nScene must contain at least one object\n");
		return (0);
	}
	return (1);
}
