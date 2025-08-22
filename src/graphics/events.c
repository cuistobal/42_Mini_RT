/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/19 09:04:20 by chrleroy         ###   ########.fr       */
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

//
int	handle_mouse(int keycode, int x, int y, void *param)
{
	t_minirt	*rt;
	double		factor;

	(void) x;
	(void) y;
	rt = (t_minirt *)param;
	if (!rt)
		return (1);
	factor = ((keycode == 5) - (keycode == 4)) * 10.0;
	return (zoom_camera(&rt->scene.camera, factor), render_scene(rt), 0);
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
int	handle_keypress(int keycode, t_minirt *rt)
{
	int	idx;
	int valid;

	idx = keycode_to_action(keycode);
	valid = (unsigned)idx < (unsigned)KEY_ACTION_COUNT;
	valid && (rt->keys[idx] = 1);
	if (keycode == KEY_ESC)
		return (exit_success(rt), 1);
	return (0);
}

// Handler pour KeyRelease : désactive la touche
int	handle_keyrelease(int keycode, t_minirt *rt)
{
	int	idx;

	idx = keycode_to_action(keycode);
	if (idx >= 0 && idx < KEY_ACTION_COUNT)
		rt->keys[idx] = 0;
	return (0);
}

// Setup des hooks MLX
void	setup_hooks(t_minirt *rt)
{
	if (!rt || !rt->mlx.mlx_ptr || !rt->mlx.win_ptr)
		return ;
	mlx_hook(rt->mlx.win_ptr, 2, 1L << 0, handle_keypress, rt);
	mlx_hook(rt->mlx.win_ptr, 3, 1L << 1, handle_keyrelease, rt);
	mlx_hook(rt->mlx.win_ptr, 4, 1L << 2, handle_mouse, rt);
	mlx_hook(rt->mlx.win_ptr, 17, 1L << 17, handle_close, rt);
	mlx_loop_hook(rt->mlx.mlx_ptr, handle_loop, rt);
}