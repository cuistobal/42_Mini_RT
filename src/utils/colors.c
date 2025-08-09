/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	color_new(int r, int g, int b)
{
	t_color	color;

	color.r = r;
	color.g = g;
	color.b = b;
	return (color);
}

t_color	color_add(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r + b.r;
	result.g = a.g + b.g;
	result.b = a.b + b.b;
	return (color_clamp(result));
}

t_color	color_mult(t_color color, double scalar)
{
	t_color	result;

	result.r = (int)(color.r * scalar);
	result.g = (int)(color.g * scalar);
	result.b = (int)(color.b * scalar);
	return (color_clamp(result));
}

t_color	color_blend(t_color a, t_color b, double ratio)
{
	t_color	result;

	result.r = (int)(a.r * (1.0 - ratio) + b.r * ratio);
	result.g = (int)(a.g * (1.0 - ratio) + b.g * ratio);
	result.b = (int)(a.b * (1.0 - ratio) + b.b * ratio);
	return (color_clamp(result));
}

t_color	color_clamp(t_color color)
{
	t_color	result;

	result.r = (color.r > 255) ? 255 : (color.r < 0) ? 0 : color.r;
	result.g = (color.g > 255) ? 255 : (color.g < 0) ? 0 : color.g;
	result.b = (color.b > 255) ? 255 : (color.b < 0) ? 0 : color.b;
	return (result);
}/*

** color_gamma_correct - Apply gamma correction to color
** color: Input color to gamma correct
** gamma: Gamma value (typically 2.2 for standard displays)
** Returns: Gamma corrected color
*/
t_color	color_gamma_correct(t_color color, double gamma)
{
	t_color	result;
	double	inv_gamma;

	if (gamma <= 0.0)
		return (color);
	
	inv_gamma = 1.0 / gamma;
	
	// Apply gamma correction: output = input^(1/gamma)
	result.r = (int)(255.0 * pow((double)color.r / 255.0, inv_gamma));
	result.g = (int)(255.0 * pow((double)color.g / 255.0, inv_gamma));
	result.b = (int)(255.0 * pow((double)color.b / 255.0, inv_gamma));
	
	return (color_clamp(result));
}

/*
** color_linear_to_srgb - Convert linear RGB to sRGB color space
** color: Linear RGB color
** Returns: sRGB color for display
*/
t_color	color_linear_to_srgb(t_color color)
{
	t_color	result;
	double	r, g, b;

	// Normalize to [0, 1] range
	r = (double)color.r / 255.0;
	g = (double)color.g / 255.0;
	b = (double)color.b / 255.0;
	
	// Apply sRGB gamma curve
	if (r <= 0.0031308)
		r = 12.92 * r;
	else
		r = 1.055 * pow(r, 1.0 / 2.4) - 0.055;
	
	if (g <= 0.0031308)
		g = 12.92 * g;
	else
		g = 1.055 * pow(g, 1.0 / 2.4) - 0.055;
	
	if (b <= 0.0031308)
		b = 12.92 * b;
	else
		b = 1.055 * pow(b, 1.0 / 2.4) - 0.055;
	
	// Convert back to [0, 255] range
	result.r = (int)(r * 255.0);
	result.g = (int)(g * 255.0);
	result.b = (int)(b * 255.0);
	
	return (color_clamp(result));
}