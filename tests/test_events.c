/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"

/*
** Test basic event handling functionality
*/
int main(void)
{
    t_minirt rt;
    
    // Initialize basic structures
    rt.mlx.mlx_ptr = NULL;
    rt.mlx.win_ptr = NULL;
    rt.scene.camera.position = vec3_new(0, 0, 0);
    rt.scene.camera.direction = vec3_new(0, 0, 1);
    rt.scene.camera.up = vec3_new(0, 1, 0);
    rt.scene.camera.right = vec3_new(1, 0, 0);
    rt.scene.camera.fov = 70;
    
    printf("Testing event handling system...\n");
    
    printf("Testing movement keys...\n");
    t_vec3 original_pos = rt.scene.camera.position;
    
    // Test W key (forward movement)
    printf("Original position: (%.2f, %.2f, %.2f)\n", 
           original_pos.x, original_pos.y, original_pos.z);
    
    // Test movement without calling handle_keypress to avoid render_scene call
    printf("Testing camera movement functions directly...\n");
    
    // Test forward movement
    move_camera(&rt.scene.camera, rt.scene.camera.direction, 1.0);
    printf("After forward movement: (%.2f, %.2f, %.2f)\n", 
           rt.scene.camera.position.x, rt.scene.camera.position.y, rt.scene.camera.position.z);
    
    // Test backward movement
    move_camera(&rt.scene.camera, vec3_mult(rt.scene.camera.direction, -1), 1.0);
    printf("After backward movement: (%.2f, %.2f, %.2f)\n", 
           rt.scene.camera.position.x, rt.scene.camera.position.y, rt.scene.camera.position.z);
    
    // Test left movement
    move_camera(&rt.scene.camera, vec3_mult(rt.scene.camera.right, -1), 1.0);
    printf("After left movement: (%.2f, %.2f, %.2f)\n", 
           rt.scene.camera.position.x, rt.scene.camera.position.y, rt.scene.camera.position.z);
    
    // Test right movement
    move_camera(&rt.scene.camera, rt.scene.camera.right, 1.0);
    printf("After right movement: (%.2f, %.2f, %.2f)\n", 
           rt.scene.camera.position.x, rt.scene.camera.position.y, rt.scene.camera.position.z);
    
    printf("Testing rotation functions...\n");
    t_vec3 original_dir = rt.scene.camera.direction;
    printf("Original direction: (%.2f, %.2f, %.2f)\n", 
           original_dir.x, original_dir.y, original_dir.z);
    
    // Test rotation
    rotate_camera(&rt.scene.camera, 0.1, 0);
    printf("After yaw rotation: (%.2f, %.2f, %.2f)\n", 
           rt.scene.camera.direction.x, rt.scene.camera.direction.y, rt.scene.camera.direction.z);
    
    rotate_camera(&rt.scene.camera, 0, 0.1);
    printf("After pitch rotation: (%.2f, %.2f, %.2f)\n", 
           rt.scene.camera.direction.x, rt.scene.camera.direction.y, rt.scene.camera.direction.z);
    
    printf("Testing event hook setup...\n");
    setup_hooks(&rt);
    printf("Hook setup completed (no MLX context, so no actual hooks set)\n");
    
    printf("Event handling test completed successfully!\n");
    return (0);
}