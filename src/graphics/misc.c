/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:23:48 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 10:24:01 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	zoom_camera(t_camera *cam, double factor)
{
	if (!cam)
		return ;
	cam->fov += factor;
	if (cam->fov < 10.0)
		cam->fov = 10.0;
	if (cam->fov > 170.0)
		cam->fov = 170.0;
}
