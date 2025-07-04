/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:58:02 by idioumas          #+#    #+#             */
/*   Updated: 2025/07/04 18:48:43 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
    int 	bpp;
    int 	endian;
    int 	line_length;

    screen->mlxptr = mlx_init();
	if (!screen->mlxptr)
		return (false);
    screen->mlxwin = create_mlx_window(screen);
	if (!screen->mlxwin)
		return (false);
    screen->tmp_img = create_temp_image(screen);
	screen->img_add = mlx_get_data_addr(screen->tmp_img, &bpp,&line_length, &endian);
	return (true);
}
