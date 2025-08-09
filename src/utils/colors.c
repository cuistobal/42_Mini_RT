/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 10:51:00 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
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
	double	r;
	double	g;
	double	b;
	t_color	result;

	r = (double)color.r / 255.0;
	g = (double)color.g / 255.0;
	b = (double)color.b / 255.0;
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
	result.r = (int)(r * 255.0);
	result.g = (int)(g * 255.0);
	result.b = (int)(b * 255.0);
	return (color_clamp(result));
}
