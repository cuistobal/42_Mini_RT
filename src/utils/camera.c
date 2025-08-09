/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 10:40:49 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** update_camera_vectors - Update camera coordinate system vectors
** camera: Camera structure to update
** 
** This function recalculates the right and up vectors based on the
** current direction vector to maintain a consistent coordinate system.
*/
void	update_camera_vectors(t_camera *camera)
{
	t_vec3	world_up;

	if (!camera)
		return ;
	world_up = vec3_new(0, 1, 0);
	camera->direction = vec3_normalize(camera->direction);
	camera->right = vec3_normalize(vec3_cross(camera->direction, world_up));
	camera->up = vec3_normalize(vec3_cross(camera->right, camera->direction));
}

/*
** move_camera - Move camera in specified direction
** camera: Camera structure to modify
** direction: Direction vector (should be normalized)
** distance: Distance to move
**
** This function moves the camera position by the specified distance
** in the given direction. The direction vector is normalized to ensure
** consistent movement speed regardless of input vector magnitude.
*/
void	move_camera(t_camera *camera, t_vec3 direction, double distance)
{
	t_vec3	movement;

	if (!camera)
		return ;
	movement = vec3_mult(vec3_normalize(direction), distance);
	camera->position = vec3_add(camera->position, movement);
}

static void	pitch_epsilon_helper(t_camera *camera, double pitch)
{
	t_vec3	new_dir;
	double	cos_pitch;
	double	sin_pitch;

	cos_pitch = cos(pitch);
	sin_pitch = sin(pitch);
	new_dir = camera->direction;
	new_dir.y = camera->direction.y * cos_pitch - \
				camera->direction.z * sin_pitch;
	new_dir.z = camera->direction.y * sin_pitch + \
				camera->direction.z * cos_pitch;
	camera->direction = new_dir;		
}

static void	yaw_epsilon_helper(t_camera *camera, double yaw)
{
	t_vec3	new_dir;
	double	cos_yaw;
	double	sin_yaw;

	cos_yaw = cos(yaw);
	sin_yaw = sin(yaw);
	new_dir.x = camera->direction.x * cos_yaw - camera->direction.z * sin_yaw;
	new_dir.y = camera->direction.y;
	new_dir.z = camera->direction.x * sin_yaw + camera->direction.z * cos_yaw;
	camera->direction = new_dir;		
}

/*
** rotate_camera - Rotate camera by yaw and pitch angles
** camera: Camera structure to modify
** yaw: Rotation around Y axis (left/right) in radians
** pitch: Rotation around X axis (up/down) in radians
**
** This function applies yaw and pitch rotations to the camera direction.
** After rotation, it updates the camera coordinate system vectors.
*/
void	rotate_camera(t_camera *camera, double yaw, double pitch)
{
	if (!camera)
		return ;
	if (fabs(yaw) > EPSILON)
		yaw_epsilon_helper(camera, yaw);
	if (fabs(pitch) > EPSILON)
		pitch_epsilon_helper(camera, pitch);
	update_camera_vectors(camera);
}
