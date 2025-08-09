/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx_graphics.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"
#include <assert.h>

/*
** Test MLX initialization
*/
void	test_mlx_init(void)
{
	t_mlx	mlx;
	int		result;

	printf("Testing MLX initialization...\n");
	
	// Initialize MLX
	result = init_mlx(&mlx);
	assert(result == 0);
	
	// Check that all pointers are valid
	assert(mlx.mlx_ptr != NULL);
	assert(mlx.win_ptr != NULL);
	assert(mlx.img_ptr != NULL);
	assert(mlx.img_data != NULL);
	
	// Check dimensions
	assert(mlx.width == WIDTH);
	assert(mlx.height == HEIGHT);
	
	// Check image properties
	assert(mlx.bits_per_pixel > 0);
	assert(mlx.size_line > 0);
	
	printf("✓ MLX initialization successful\n");
	
	// Clean up
	cleanup_mlx(&mlx);
	printf("✓ MLX cleanup successful\n");
}

/*
** Test color conversion functions
*/
void	test_color_conversion(void)
{
	t_color	color, converted_back;
	int		color_int;

	printf("Testing color conversion...\n");
	
	// Test basic colors
	color.r = 255;
	color.g = 0;
	color.b = 0;
	color_int = color_to_int(color);
	assert(color_int == 0xFF0000); // Red
	
	color.r = 0;
	color.g = 255;
	color.b = 0;
	color_int = color_to_int(color);
	assert(color_int == 0x00FF00); // Green
	
	color.r = 0;
	color.g = 0;
	color.b = 255;
	color_int = color_to_int(color);
	assert(color_int == 0x0000FF); // Blue
	
	// Test white
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color_int = color_to_int(color);
	assert(color_int == 0xFFFFFF);
	
	// Test black
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color_int = color_to_int(color);
	assert(color_int == 0x000000);
	
	// Test reverse conversion
	converted_back = int_to_color(0xFF8040);
	assert(converted_back.r == 255);
	assert(converted_back.g == 128);
	assert(converted_back.b == 64);
	
	// Test clamping (values outside [0, 255])
	color.r = 300;  // Should clamp to 255
	color.g = -50;  // Should clamp to 0
	color.b = 128;
	color_int = color_to_int(color);
	assert(color_int == 0xFF0080);
	
	printf("✓ Color conversion tests passed\n");
}

/*
** Test pixel manipulation (without actually displaying)
*/
void	test_pixel_operations(void)
{
	t_mlx	mlx;
	t_color	red, green, blue;

	printf("Testing pixel operations...\n");
	
	// Initialize MLX
	assert(init_mlx(&mlx) == 0);
	
	// Define test colors
	red.r = 255; red.g = 0; red.b = 0;
	green.r = 0; green.g = 255; green.b = 0;
	blue.r = 0; blue.g = 0; blue.b = 255;
	
	// Test putting pixels (should not crash)
	put_pixel(&mlx, 0, 0, red);
	put_pixel(&mlx, WIDTH-1, HEIGHT-1, green);
	put_pixel(&mlx, WIDTH/2, HEIGHT/2, blue);
	
	// Test boundary conditions (should not crash)
	put_pixel(&mlx, -1, -1, red);        // Out of bounds
	put_pixel(&mlx, WIDTH, HEIGHT, red); // Out of bounds
	put_pixel(&mlx, WIDTH/2, -1, red);   // Out of bounds
	put_pixel(&mlx, -1, HEIGHT/2, red);  // Out of bounds
	
	printf("✓ Pixel operations tests passed\n");
	
	// Clean up
	cleanup_mlx(&mlx);
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== MLX Graphics System Tests ===\n\n");
	
	test_color_conversion();
	test_mlx_init();
	test_pixel_operations();
	
	printf("\n=== All MLX graphics tests passed! ===\n");
	printf("Note: Window display and event handling require interactive testing.\n");
	
	return (0);
}