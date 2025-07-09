/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:58:02 by idioumas          #+#    #+#             */
/*   Updated: 2025/07/05 13:26:50 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//
bool	init_window(t_screen *screen)
{
	int		width;
	int		height;
	void	*mlxptr;

    screen->mlxptr = mlx_init();
	if (!screen->mlxptr)
		return (false);
	mlx_get_screen_size(screen->mlxptr, &screen->width, &screen->height);
	width = screen->width;
	height = screen->height;
	mlxptr = screen->mlxptr;
    screen->mlxwin = mlx_new_window(mlxptr, width, height, PROGRAM_NAME);
	return (screen->mlxwin);
}
