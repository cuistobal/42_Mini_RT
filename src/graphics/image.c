/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 12:50:50 by chrleroy         ###   ########.fr       */
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
	int		color_int;

	if (!mlx || !mlx->img_data)
		return ;
	if (x < 0 || x >= mlx->width || y < 0 || y >= mlx->height)
		return ;
	pixel = mlx->img_data + (y * mlx->size_line + x * \
			(mlx->bits_per_pixel / 8));
	//Debug
	if (color.r < 0 || color.r > 255 || \
		color.g < 0 || color.g > 255 || color.b < 0 || color.b > 255)
	{
		printf("%s: %d %d %d\n", __func__, color.r, color.g, color.b);
	}

	color_int = color_to_int(color);
	*(unsigned int *)pixel = color_int;
}

/*
** color_to_int - Convert RGB color structure to integer format
** Returns color as 32-bit integer (0x00RRGGBB format)
*/
int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = color.r;
	if (r > 255)
		r = 255;
	else if (r < 0)
		r = 0;
	g = color.g;
	if (g > 255)
		g = 255;
	else if (g < 0)
		g = 0;
	b = color.b;
	if (b > 255)
		b = 255;
	else if (b < 0)
		b = 0;
	return ((r << RED_OFFSET) | (g << GREEN_OFFSET) | b);
}

/*
** int_to_color - Convert integer color to RGB color structure
** color_int: 32-bit color integer (0x00RRGGBB format)
** Returns RGB color structure
*/
t_color	int_to_color(int color_int)
{
	t_color	color;

	color.r = (color_int >> RED_OFFSET) & 0xFF;
	color.g = (color_int >> GREEN_OFFSET) & 0xFF;
	color.b = color_int & 0xFF;
	return (color);
}

/*
** display_image - Display the rendered image buffer to the window
*/
void	display_image(t_mlx *mlx)
{
	if (!mlx || !mlx->mlx_ptr || !mlx->win_ptr || !mlx->img_ptr)
		return ;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}
