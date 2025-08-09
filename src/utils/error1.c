/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:30:14 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:30:35 by chrleroy         ###   ########.fr       */
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
	if (!filename)
		filename = "unkmown";
	if (!operation)
		operation = "operation";
	printf("Error\nFile %s: %s failed\n", filename, operation);
	if (rt)
		cleanup_all(rt);
	exit(1);
}
