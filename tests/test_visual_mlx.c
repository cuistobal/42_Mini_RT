/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_visual_mlx.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"

/*
** Simple visual test - draws a gradient and some colored squares
*/
void	draw_test_pattern(t_mlx *mlx)
{
	int		x, y;
	t_color	color;

	// Draw a simple gradient background
	for (y = 0; y < mlx->height; y++)
	{
		for (x = 0; x < mlx->width; x++)
		{
			color.r = (x * 255) / mlx->width;
			color.g = (y * 255) / mlx->height;
			color.b = 128;
			put_pixel(mlx, x, y, color);
		}
	}
	
	// Draw some colored squares
	t_color red = color_new(255, 0, 0);
	t_color green = color_new(0, 255, 0);
	t_color blue = color_new(0, 0, 255);
	t_color white = color_new(255, 255, 255);
	
	// Red square (top-left)
	for (y = 50; y < 150; y++)
		for (x = 50; x < 150; x++)
			put_pixel(mlx, x, y, red);
	
	// Green square (top-right)
	for (y = 50; y < 150; y++)
		for (x = mlx->width - 150; x < mlx->width - 50; x++)
			put_pixel(mlx, x, y, green);
	
	// Blue square (bottom-left)
	for (y = mlx->height - 150; y < mlx->height - 50; y++)
		for (x = 50; x < 150; x++)
			put_pixel(mlx, x, y, blue);
	
	// White square (center)
	for (y = mlx->height/2 - 50; y < mlx->height/2 + 50; y++)
		for (x = mlx->width/2 - 50; x < mlx->width/2 + 50; x++)
			put_pixel(mlx, x, y, white);
}

/*
** Key handler for visual test
*/
int	test_key_handler(int keycode, t_mlx *mlx)
{
	if (keycode == KEY_ESC)
	{
		cleanup_mlx(mlx);
		exit(0);
	}
	return (0);
}

/*
** Close handler for visual test
*/
int	test_close_handler(t_mlx *mlx)
{
	cleanup_mlx(mlx);
	exit(0);
	return (0);
}

/*
** Main visual test function
*/
int	main(void)
{
	t_mlx	mlx;

	printf("Starting MLX visual test...\n");
	printf("Press ESC or close window to exit.\n");
	
	// Initialize MLX
	if (init_mlx(&mlx) != 0)
	{
		printf("Error: Failed to initialize MLX\n");
		return (1);
	}
	
	// Draw test pattern
	draw_test_pattern(&mlx);
	
	// Display the image
	display_image(&mlx);
	
	// Set up event handlers
	mlx_key_hook(mlx.win_ptr, test_key_handler, &mlx);
	mlx_hook(mlx.win_ptr, 17, 1L << 17, test_close_handler, &mlx);
	
	// Start event loop
	mlx_loop(mlx.mlx_ptr);
	
	return (0);
}