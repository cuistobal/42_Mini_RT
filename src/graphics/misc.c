/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:23:48 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/19 09:03:43 by chrleroy         ###   ########.fr       */
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

int	keycode_to_action(int keycode)
{
	if (keycode == KEY_W)
		return (KEY_ACTION_W);
	if (keycode == KEY_S)
		return (KEY_ACTION_S);
	if (keycode == KEY_A)
		return (KEY_ACTION_A);
	if (keycode == KEY_D)
		return (KEY_ACTION_D);
	if (keycode == KEY_UP)
		return (KEY_ACTION_UP);
	if (keycode == KEY_DOWN)
		return (KEY_ACTION_DOWN);
	if (keycode == KEY_LEFT)
		return (KEY_ACTION_LEFT);
	if (keycode == KEY_RIGHT)
		return (KEY_ACTION_RIGHT);
	if (keycode == KEY_ESC)
		return (KEY_ACTION_ESC);
	return (-1);
}

// Boucle principale : applique le mouvement continu
int	handle_loop(t_minirt *rt)
{
	t_camera	*c;

	if (!rt)
		return (1);
	c = &rt->scene.camera;
	if (rt->keys[KEY_ACTION_W])
		move_camera(c, c->direction, c->move_speed);
	if (rt->keys[KEY_ACTION_S])
		move_camera(c, vec3_mult(c->direction, -1), c->move_speed);
	if (rt->keys[KEY_ACTION_A])
		move_camera(c, vec3_mult(c->right, -1), c->move_speed);
	if (rt->keys[KEY_ACTION_D])
		move_camera(c, c->right, c->move_speed);
	if (rt->keys[KEY_ACTION_UP])
		rotate_camera(c, 0, -c->rotate_speed);
	if (rt->keys[KEY_ACTION_DOWN])
		rotate_camera(c, 0, c->rotate_speed);
	if (rt->keys[KEY_ACTION_LEFT])
		rotate_camera(c, -c->rotate_speed, 0);
	if (rt->keys[KEY_ACTION_RIGHT])
		rotate_camera(c, c->rotate_speed, 0);
	return (render_scene(rt), 0);
}
