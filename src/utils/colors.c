/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/12 10:00:05 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	color_new(int r, int g, int b)
{
    return (t_color){r, g, b};
}

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

// Helper
static double linear_to_srgb_channel(double c)
{
    if (c <= 0.0031308)
        return 12.92 * c;
    return 1.055 * pow(c, 1.0 / 2.4) - 0.055;
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

    r = linear_to_srgb_channel(r);
    g = linear_to_srgb_channel(g);
    b = linear_to_srgb_channel(b);
    result.r = (int)(r * 255.0);
    result.g = (int)(g * 255.0);
    result.b = (int)(b * 255.0);
    return (color_clamp(result));
}
