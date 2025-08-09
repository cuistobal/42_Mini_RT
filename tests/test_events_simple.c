/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_events_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"

/*
** Simple test for camera movement and rotation functions
*/
int main(void)
{
    t_camera camera;
    
    // Initialize camera
    camera.position = vec3_new(0, 0, 0);
    camera.direction = vec3_new(0, 0, 1);
    camera.up = vec3_new(0, 1, 0);
    camera.right = vec3_new(1, 0, 0);
    camera.fov = 70;
    
    printf("Testing camera movement and rotation functions...\n");
    
    printf("Initial position: (%.2f, %.2f, %.2f)\n", 
           camera.position.x, camera.position.y, camera.position.z);
    printf("Initial direction: (%.2f, %.2f, %.2f)\n", 
           camera.direction.x, camera.direction.y, camera.direction.z);
    
    // Test forward movement
    move_camera(&camera, camera.direction, 1.0);
    printf("After forward movement: (%.2f, %.2f, %.2f)\n", 
           camera.position.x, camera.position.y, camera.position.z);
    
    // Test backward movement
    move_camera(&camera, vec3_mult(camera.direction, -1), 1.0);
    printf("After backward movement: (%.2f, %.2f, %.2f)\n", 
           camera.position.x, camera.position.y, camera.position.z);
    
    // Test left movement
    move_camera(&camera, vec3_mult(camera.right, -1), 1.0);
    printf("After left movement: (%.2f, %.2f, %.2f)\n", 
           camera.position.x, camera.position.y, camera.position.z);
    
    // Test right movement
    move_camera(&camera, camera.right, 1.0);
    printf("After right movement: (%.2f, %.2f, %.2f)\n", 
           camera.position.x, camera.position.y, camera.position.z);
    
    printf("Testing rotation...\n");
    
    // Test yaw rotation (left/right)
    rotate_camera(&camera, 0.1, 0);
    printf("After yaw rotation: (%.2f, %.2f, %.2f)\n", 
           camera.direction.x, camera.direction.y, camera.direction.z);
    
    // Test pitch rotation (up/down)
    rotate_camera(&camera, 0, 0.1);
    printf("After pitch rotation: (%.2f, %.2f, %.2f)\n", 
           camera.direction.x, camera.direction.y, camera.direction.z);
    
    printf("Camera functions test completed successfully!\n");
    return (0);
}