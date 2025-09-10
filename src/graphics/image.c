/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 08:16:16 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** put_pixel - Set a pixel in the image buffer to a specific color
** x, y: pixel coordinates (0,0 is top-left)
** color: RGB color to set
*/
void	put_pixel(t_mlx *mlx, int x, int y, t_color color)
{
	char	*pixel;

	// Vérification supplémentaire pour éviter le segfault
	if (!mlx || !mlx->img_data || x < 0 || y < 0 || 
		x >= mlx->width || y >= mlx->height)
		return;

	pixel = mlx->img_data + (y * mlx->size_line + x * \
			(mlx->bits_per_pixel / 8));
	*(unsigned int *)pixel = color_to_int(color);
}

/*
** color_to_int - Convert RGB color structure to integer format
** Returns color as 32-bit integer (0x00RRGGBB format)
*/
int	color_to_int(t_color color)
{
	return ((color.r << RED_OFFSET) | (color.g << GREEN_OFFSET) | color.b);
}

/*
** int_to_color - Convert integer color to RGB color structure
** color_int: 32-bit color integer (0x00RRGGBB format)
** Returns RGB color structure
*/
t_color	int_to_color(int color_int)
{
	return ((t_color){
		.r = color_int >> RED_OFFSET & 0xFF,
		.g = color_int >> GREEN_OFFSET & 0xFF,
		.b = color_int & 0xFF
	});
}

/*
** display_image - Display the rendered image buffer to the window
*/
void	display_image(t_mlx *mlx)
{
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}
