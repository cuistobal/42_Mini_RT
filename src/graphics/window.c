/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/13 13:57:41 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	cleanup_mlx(t_mlx *mlx)
{
	if (!mlx)
		return ;
	if (mlx->img_ptr)
		mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	if (mlx->win_ptr)
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	if (mlx->mlx_ptr)
		mlx_destroy_display(mlx->mlx_ptr);
	if (mlx->mlx_ptr)
		free(mlx->mlx_ptr);
}

/*
** init_mlx - Initialize MLX library, create window and image buffer
** Returns 0 on success, -1 on error
*/
int	init_mlx(t_mlx *mlx, char *name)
{
	if (!mlx)
		return (-1);
	mlx->mlx_ptr = mlx_init();
	if (!mlx->mlx_ptr)
		return (-1);
	mlx_get_screen_size(mlx->mlx_ptr, &mlx->width, &mlx->height);
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, mlx->width, \
			mlx->height, name);
	if (!mlx->win_ptr)
		return (cleanup_mlx(mlx), -1);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->width, mlx->height);
	if (!mlx->img_ptr)
		return (cleanup_mlx(mlx), -1);
	mlx->img_data = mlx_get_data_addr(mlx->img_ptr, &mlx->bits_per_pixel, \
			&mlx->size_line, &mlx->endian);
	if (!mlx->img_data)
		return (cleanup_mlx(mlx), -1);
	return (0);
}
