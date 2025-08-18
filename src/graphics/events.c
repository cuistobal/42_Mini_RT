/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/18 10:24:07 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** handle_keypress - Handle keyboard input events
** keycode: Key code from MLX
** rt: Main program structure
** Returns 0 to continue, 1 to exit
*/
int	handle_keypress(int keycode, t_minirt *rt)
{
	if (!rt)
		return (1);
	if (keycode == KEY_ESC)
		return (exit_success(rt), 1);
	else if (keycode == KEY_W)
		move_camera(&rt->scene.camera, rt->scene.camera.direction, \
				rt->scene.camera.move_speed);
	else if (keycode == KEY_S)
		move_camera(&rt->scene.camera, vec3_mult(rt->scene.camera.direction, \
					-1), rt->scene.camera.move_speed);
	else if (keycode == KEY_A)
		move_camera(&rt->scene.camera, vec3_mult(rt->scene.camera.right, -1), \
				rt->scene.camera.move_speed);
	else if (keycode == KEY_D)
		move_camera(&rt->scene.camera, rt->scene.camera.right, \
				rt->scene.camera.move_speed);
	else if (keycode == KEY_UP)
		rotate_camera(&rt->scene.camera, 0, -rt->scene.camera.rotate_speed);
	else if (keycode == KEY_DOWN)
		rotate_camera(&rt->scene.camera, 0, rt->scene.camera.rotate_speed);
	else if (keycode == KEY_LEFT)
		rotate_camera(&rt->scene.camera, -rt->scene.camera.rotate_speed, 0);
	else if (keycode == KEY_RIGHT)
		rotate_camera(&rt->scene.camera, rt->scene.camera.rotate_speed, 0);
	return (render_scene(rt), 0);
}

int	handle_mouse(int keycode, int x, int y, void *param)
{
	t_minirt	*rt;

	(void) x;
	(void) y;
	rt = (t_minirt *)param;
	if (!rt)
		return (1);
	if (keycode == 4)
		zoom_camera(&rt->scene.camera, -10);
	else if (keycode == 5)
		zoom_camera(&rt->scene.camera, 10);
	return (render_scene(rt), 0);
}

/*
** handle_close - Handle window close event (X button)
** rt: Main program structure
** Returns 1 to exit
*/
int	handle_close(t_minirt *rt)
{
	if (!rt)
		return (1);
	exit_success(rt);
	return (1);
}

/*
** handle_loop - Main event loop handler (called continuously)
** rt: Main program structure
** Returns 0 to continue
** This function can be used for continuous updates
** For now, it just returns to keep the loop running
*/
int	handle_loop(t_minirt *rt)
{
	return (!rt);
}

/*
** setup_hooks - Set up MLX event hooks
** rt: Main program structure
*/
void	setup_hooks(t_minirt *rt)
{
	if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
		return ;
	mlx_hook(rt->mlx.win_ptr, 4, 1L << 2, handle_mouse, rt);
	mlx_key_hook(rt->mlx.win_ptr, handle_keypress, rt);
	mlx_hook(rt->mlx.win_ptr, 17, 1L << 17, handle_close, rt);
	mlx_loop_hook(rt->mlx.mlx_ptr, handle_loop, rt);
}

/*
** Event handling documentation:
** 
** Key mappings:
** - ESC: Exit program
** - W: Move camera forward
** - S: Move camera backward  
** - A: Move camera left
** - D: Move camera right
** - UP: Rotate camera up
** - DOWN: Rotate camera down
** - LEFT: Rotate camera left
** - RIGHT: Rotate camera right
**
** The system supports real-time camera movement and rotation
** with automatic scene re-rendering after each movement.
*/
