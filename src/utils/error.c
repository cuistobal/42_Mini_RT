/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	error_exit(char *message, t_minirt *rt)
{
	printf("Error\n%s\n", message);
	if (rt)
		cleanup_all(rt);
	exit(1);
}

/*
** exit_success - Exit program successfully with proper cleanup
** rt: Main program structure for cleanup
*/
void	exit_success(t_minirt *rt)
{
	if (rt)
		cleanup_all(rt);
	exit(0);
}

/*
** print_error - Print error message with proper "Error\n" prefix
** message: Error message to display
*/
void	print_error(char *message)
{
	if (message)
		printf("Error\n%s\n", message);
	else
		printf("Error\nUnknown error occurred\n");
}

/*
** handle_malloc_error - Handle memory allocation errors
** size: Size that failed to allocate
** rt: Main program structure for cleanup
*/
void	handle_malloc_error(size_t size, t_minirt *rt)
{
	printf("Error\nMemory allocation failed for %zu bytes\n", size);
	if (rt)
		cleanup_all(rt);
	exit(1);
}

/*
** handle_file_error - Handle file operation errors
** filename: Name of file that caused error
** operation: Type of operation that failed
** rt: Main program structure for cleanup
*/
void	handle_file_error(char *filename, char *operation, t_minirt *rt)
{
	printf("Error\nFile %s: %s failed\n", 
		filename ? filename : "unknown", 
		operation ? operation : "operation");
	if (rt)
		cleanup_all(rt);
	exit(1);
}

/*
** handle_parse_error - Handle parsing errors with line information
** filename: Name of file being parsed
** line_num: Line number where error occurred
** message: Specific error message
** rt: Main program structure for cleanup
*/
void	handle_parse_error(char *filename, int line_num, char *message, t_minirt *rt)
{
	printf("Error\nParsing error in %s at line %d: %s\n", 
		filename ? filename : "unknown file", 
		line_num, 
		message ? message : "syntax error");
	if (rt)
		cleanup_all(rt);
	exit(1);
}

void	cleanup_scene(t_scene *scene)
{
	if (!scene)
		return ;
	
	// Use the dedicated cleanup functions for better memory management
	cleanup_light_list(scene->lights);
	cleanup_object_list(scene->objects);
	cleanup_bvh(scene->bvh_root);
	
	// Reset pointers to NULL to prevent double-free
	scene->lights = NULL;
	scene->objects = NULL;
	scene->bvh_root = NULL;
}

void	cleanup_all(t_minirt *rt)
{
	if (!rt)
		return ;
	cleanup_scene(&rt->scene);
	cleanup_mlx(&rt->mlx);
}

int	validate_scene(t_scene *scene)
{
	if (!scene)
	{
		printf("Error\nInvalid scene structure\n");
		return (0);
	}
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