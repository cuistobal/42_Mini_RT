/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:58:02 by idioumas          #+#    #+#             */
/*   Updated: 2025/07/04 16:35:57 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
# include <math.h>

//
void mlx_put_color(char *img_addr, int x, int y, int color, int line_len, int bpp)
{
    // Calculate memory offset to the pixel
    int offset = y * line_len + x * (bpp / 8);

    // Write the color into the image buffer
    *(unsigned int *)(img_addr + offset) = color;
}

//
void	*create_temp_image(t_screen	*screen)
{	
	int		width;
	int		height;
	void	*mlxptr;

	width = screen->width;
	height = screen->height;
	mlxptr = screen->mlxptr;
	return (mlx_new_image(mlxptr, width, height));
}

//
void	*create_mlx_window(t_screen *screen)
{
	int		width;
	int		height;
	void	*mlxptr;

	width = screen->width;
	height = screen->height;
	mlxptr = screen->mlxptr;
	return (mlx_new_window(mlxptr, width, height, "MiniRT"));
}

//
bool	init_window(t_screen *screen)
{
    void	*tmp_img;
    void	*img_addr;
    int 	bpp;
    int 	line_length;
    int 	endian;

    screen->mlxptr = mlx_init();
	if (!screen->mlxptr)
		return (false);
    screen->mlxwin = create_mlx_window(screen);
	if (!screen->mlxwin)
		return (false);
    tmp_img = create_temp_image(screen);
	img_addr = mlx_get_data_addr(tmp_img, &bpp,&line_length, &endian);
	return (true);
}
