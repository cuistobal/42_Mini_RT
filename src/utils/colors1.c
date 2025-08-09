/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:47:46 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/09 12:15:27 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

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

static void	check_limits(int *color)
{
	if (*color > 255)
		*color = 255;
	else if (*color < 0)
		*color = 0;
}

t_color	color_clamp(t_color color)
{
	t_color	result;

	result.r = color.r;
	result.g = color.g;
	result.b = color.b;
	check_limits(&result.r);
	check_limits(&result.g);
	check_limits(&result.b);
	return (result);
}
