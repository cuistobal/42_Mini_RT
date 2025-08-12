/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:47:46 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/12 15:34:03 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	color_add(t_color a, t_color b)
{
	return (t_color){a.r + b.r, a.g + b.g, a.b + b.b};
}

t_color	color_mult(t_color color, double scalar)
{
	return (t_color){
		(int)(color.r * scalar),
		(int)(color.g * scalar),
		(int)(color.b * scalar)
	};
}

t_color	color_blend(t_color a, t_color b, double ratio)
{
	return (t_color){
		(int)(a.r * (1.0 - ratio) + b.r * ratio),
		(int)(a.g * (1.0 - ratio) + b.g * ratio),
		(int)(a.b * (1.0 - ratio) + b.b * ratio)
	};
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
