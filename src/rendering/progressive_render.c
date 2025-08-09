/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progressive_render.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/07/30 07:37:53 by cuistobal        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** fill_pixel_block - Fill a block of pixels with the same color
*/
void	fill_pixel_block(t_minirt *rt, int x, int y, int step,
	t_color pixel_color)
{
	int	block_x;
	int	block_y;

	block_y = y;
	while (block_y < y + step && block_y < rt->mlx.height)
	{
		block_x = x;
		while (block_x < x + step && block_x < rt->mlx.width)
		{
			put_pixel(&rt->mlx, block_x, block_y, pixel_color);
			block_x++;
		}
		block_y++;
	}
}

/*
** render_progressive_step - Render one step of progressive rendering
*/
void	render_progressive_step(t_minirt *rt, int step,
	double inv_width, double inv_height)
{
	int		x;
	int		y;
	double	u;
	double	v;
	t_ray	ray;
	t_color	pixel_color;

	y = 0;
	while (y < rt->mlx.height)
	{
		x = 0;
		while (x < rt->mlx.width)
		{
			if (x % step == 0 && y % step == 0)
			{
				u = (double)x * inv_width;
				v = (double)y * inv_height;
				ray = get_camera_ray(rt, &rt->scene.camera, u, v);
				pixel_color = raycast_optimized(ray, &rt->scene, MAX_DEPTH);
				fill_pixel_block(rt, x, y, step, pixel_color);
			}
			x += step;
		}
		y += step;
	}
}

/*
** render_scene_progressive - Progressive rendering with visual feedback
*/
void	render_scene_progressive(t_minirt *rt)
{
	int		step;
	double	inv_width;
	double	inv_height;

	if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
		return ;
	setup_camera(&rt->scene.camera);
	inv_width = 1.0 / (double)rt->mlx.width;
	inv_height = 1.0 / (double)rt->mlx.height;
	step = 8;
	while (step >= 1)
	{
		printf("Rendering at step size %d...\n", step);
		render_progressive_step(rt, step, inv_width, inv_height);
		display_image(&rt->mlx);
		step /= 2;
	}
	printf("Progressive rendering complete!\n");
}