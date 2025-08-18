/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/18 14:45:11 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"


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
int keycode_to_action(int keycode)
{
    if (keycode == KEY_W)
		return KEY_ACTION_W;
    if (keycode == KEY_S)
		return KEY_ACTION_S;
    if (keycode == KEY_A)
		return KEY_ACTION_A;
    if (keycode == KEY_D)
		return KEY_ACTION_D;
	if (keycode == KEY_UP)
		return KEY_ACTION_UP;
	if (keycode == KEY_DOWN)
		return KEY_ACTION_DOWN;
	if (keycode == KEY_LEFT)
		return KEY_ACTION_LEFT;
	if (keycode == KEY_RIGHT)
		return KEY_ACTION_RIGHT;
	if (keycode == KEY_ESC)
		return KEY_ACTION_ESC;
	return -1;
}

//
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

// Handler pour KeyPress : active la touche
int handle_keypress(int keycode, t_minirt *rt)
{
	int	idx;

	idx = keycode_to_action(keycode);
    if (idx >= 0 && idx < KEY_ACTION_COUNT)
        rt->keys[idx] = 1;
    if (keycode == KEY_ESC)
        return (exit_success(rt), 1);
    return (0);
}

// Handler pour KeyRelease : désactive la touche
int handle_keyrelease(int keycode, t_minirt *rt)
{
    int idx;

    idx = keycode_to_action(keycode);
    if (idx >= 0 && idx < KEY_ACTION_COUNT)
        rt->keys[idx] = 0;
    return (0);
}

// Boucle principale : applique le mouvement continu
int handle_loop(t_minirt *rt)
{	
		if (rt->keys[KEY_ACTION_W])
			move_camera(&rt->scene.camera, rt->scene.camera.direction, rt->scene.camera.move_speed);
		if (rt->keys[KEY_ACTION_S])
			move_camera(&rt->scene.camera, vec3_mult(rt->scene.camera.direction, -1), rt->scene.camera.move_speed);
		if (rt->keys[KEY_ACTION_A])
			move_camera(&rt->scene.camera, vec3_mult(rt->scene.camera.right, -1), rt->scene.camera.move_speed);
		if (rt->keys[KEY_ACTION_D])
			move_camera(&rt->scene.camera, rt->scene.camera.right, rt->scene.camera.move_speed);
		if (rt->keys[KEY_ACTION_UP])
			rotate_camera(&rt->scene.camera, 0, -rt->scene.camera.rotate_speed);
		if (rt->keys[KEY_ACTION_DOWN])
			rotate_camera(&rt->scene.camera, 0, rt->scene.camera.rotate_speed);
		if (rt->keys[KEY_ACTION_LEFT])
			rotate_camera(&rt->scene.camera, -rt->scene.camera.rotate_speed, 0);
		if (rt->keys[KEY_ACTION_RIGHT])
			rotate_camera(&rt->scene.camera, rt->scene.camera.rotate_speed, 0);
		return (render_scene(rt), 0);
}

// Setup des hooks MLX
void setup_hooks(t_minirt *rt)
{
    if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
        return ;
    mlx_hook(rt->mlx.win_ptr, 2, 1L << 0, handle_keypress, rt);
    mlx_hook(rt->mlx.win_ptr, 3, 1L << 1, handle_keyrelease, rt);
	mlx_hook(rt->mlx.win_ptr, 4, 1L << 2, handle_mouse, rt);
    mlx_hook(rt->mlx.win_ptr, 17, 1L << 17, handle_close, rt);
    mlx_loop_hook(rt->mlx.mlx_ptr, handle_loop, rt);
}
