/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_temp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 11:44:26 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** move_camera - Move camera in specified direction
** camera: Camera structure to modify
** direction: Direction vector (should be normalized)
** distance: Distance to move
*/
void	move_camera(t_camera *camera, t_vec3 direction, double distance)
{
	t_vec3	movement;

	if (!camera)
		return ;
	movement = vec3_mult(vec3_normalize(direction), distance);
	camera->position = vec3_add(camera->position, movement);
}

void	pitch_epsilon_helper(t_camera *camera, double pitch)
{
	double	cos_pitch;
	double	sin_pitch;
	t_vec3	new_dir;

	cos_pitch = cos(pitch);
	sin_pitch = sin(pitch);
	new_dir.x = camera->direction.x;
	new_dir.y = camera->direction.y * cos_pitch - camera->direction.z \
				* sin_pitch;
	new_dir.z = camera->direction.y * sin_pitch + camera->direction.z \
				* cos_pitch;
	camera->direction = vec3_normalize(new_dir);
}

void	yaw_epsilon_helper(t_camera *camera, double yaw)
{
	double	cos_yaw;
	double	sin_yaw;
	t_vec3	new_dir;

	cos_yaw = cos(yaw);
	sin_yaw = sin(yaw);
	new_dir.x = camera->direction.x * cos_yaw - camera->direction.z * sin_yaw;
	new_dir.y = camera->direction.y;
	new_dir.z = camera->direction.x * sin_yaw + camera->direction.z * cos_yaw;
	camera->direction = vec3_normalize(new_dir);
}

/*
** rotate_camera - Rotate camera by yaw and pitch angles
** camera: Camera structure to modify
** yaw: Rotation around Y axis (left/right)
** pitch: Rotation around X axis (up/down)
*/
void	rotate_camera(t_camera *camera, double yaw, double pitch)
{
	t_vec3	world_up;

	if (!camera)
		return ;
	world_up = vec3_new(0, 1, 0);
	if (fabs(yaw) > EPSILON)
		yaw_epsilon_helper(camera, yaw);
	if (fabs(pitch) > EPSILON)
		pitch_epsilon_helper(camera, pitch);
	camera->right = vec3_normalize(vec3_cross(camera->direction, world_up));
	camera->up = vec3_normalize(vec3_cross(camera->right, camera->direction));
}

// render_scene function moved to src/rendering/raytracer.c
