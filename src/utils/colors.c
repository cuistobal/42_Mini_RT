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
	double	inv_gamma;

	if (gamma <= 0.0)
		return (color);
	inv_gamma = 1.0 / gamma;
	return (t_color){
		(int)(255.0 * pow((double)color.r / 255.0, inv_gamma)),
		(int)(255.0 * pow((double)color.g / 255.0, inv_gamma)),
		(int)(255.0 * pow((double)color.b / 255.0, inv_gamma))
	};
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
    double	r = linear_to_srgb_channel((double)color.r / 255.0);
    double	g = linear_to_srgb_channel((double)color.g / 255.0);
    double	b = linear_to_srgb_channel((double)color.b / 255.0);

    return (t_color){
        (int)(r * 255.0),
        (int)(g * 255.0),
        (int)(b * 255.0)
    };
}