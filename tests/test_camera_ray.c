/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_camera_ray.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"
#include <assert.h>

#define TEST_EPSILON 1e-6

/*
** Helper function to compare doubles with epsilon tolerance
*/
int	double_equals(double a, double b)
{
	return (fabs(a - b) < TEST_EPSILON);
}

/*
** Helper function to compare vectors with epsilon tolerance
*/
int	vec3_equals(t_vec3 a, t_vec3 b)
{
	return (double_equals(a.x, b.x) && 
			double_equals(a.y, b.y) && 
			double_equals(a.z, b.z));
}

/*
** Test ray_from_camera function
*/
void	test_ray_from_camera(void)
{
	t_ray	ray;
	t_vec3	camera_pos, camera_dir, camera_up, camera_right;
	double	fov;

	printf("Testing ray_from_camera...\n");
	
	// Set up standard camera looking down Z axis
	camera_pos = vec3_new(0.0, 0.0, 0.0);
	camera_dir = vec3_new(0.0, 0.0, 1.0);
	camera_up = vec3_new(0.0, 1.0, 0.0);
	camera_right = vec3_new(1.0, 0.0, 0.0);
	fov = M_PI / 3.0; // 60 degrees
	
	// Test center pixel (0.5, 0.5) - should go straight forward
	ray = ray_from_camera(camera_pos, camera_dir, camera_up, camera_right, fov, 0.5, 0.5);
	
	// Ray should start at camera position
	assert(vec3_equals(ray.origin, camera_pos));
	
	// Ray direction should be normalized
	double length = vec3_length(ray.direction);
	assert(double_equals(length, 1.0));
	
	// For center pixel, ray should go roughly in camera direction
	// (exact direction depends on FOV calculations)
	double dot_with_forward = vec3_dot(ray.direction, camera_dir);
	assert(dot_with_forward > 0.5); // Should be pointing generally forward
	
	// Test corner pixels
	ray = ray_from_camera(camera_pos, camera_dir, camera_up, camera_right, fov, 0.0, 0.0);
	assert(vec3_equals(ray.origin, camera_pos));
	assert(double_equals(vec3_length(ray.direction), 1.0));
	
	ray = ray_from_camera(camera_pos, camera_dir, camera_up, camera_right, fov, 1.0, 1.0);
	assert(vec3_equals(ray.origin, camera_pos));
	assert(double_equals(vec3_length(ray.direction), 1.0));
	
	printf("✓ ray_from_camera tests passed\n");
}

/*
** Test camera ray generation for different FOVs
*/
void	test_camera_fov_effects(void)
{
	t_ray	ray_narrow, ray_wide;
	t_vec3	camera_pos, camera_dir, camera_up, camera_right;
	double	narrow_fov, wide_fov;

	printf("Testing camera FOV effects...\n");
	
	camera_pos = vec3_new(0.0, 0.0, 0.0);
	camera_dir = vec3_new(0.0, 0.0, 1.0);
	camera_up = vec3_new(0.0, 1.0, 0.0);
	camera_right = vec3_new(1.0, 0.0, 0.0);
	
	narrow_fov = M_PI / 6.0; // 30 degrees
	wide_fov = M_PI / 2.0;   // 90 degrees
	
	// Test edge pixel (1.0, 0.5) with different FOVs
	ray_narrow = ray_from_camera(camera_pos, camera_dir, camera_up, camera_right, 
								narrow_fov, 1.0, 0.5);
	ray_wide = ray_from_camera(camera_pos, camera_dir, camera_up, camera_right, 
							  wide_fov, 1.0, 0.5);
	
	// Both rays should be normalized
	assert(double_equals(vec3_length(ray_narrow.direction), 1.0));
	assert(double_equals(vec3_length(ray_wide.direction), 1.0));
	
	// Wide FOV should have more deviation from center
	double narrow_dot = vec3_dot(ray_narrow.direction, camera_dir);
	double wide_dot = vec3_dot(ray_wide.direction, camera_dir);
	
	// Narrow FOV should be closer to forward direction
	assert(narrow_dot > wide_dot);
	
	printf("✓ Camera FOV effects tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Camera Ray Generation Tests ===\n\n");
	
	test_ray_from_camera();
	test_camera_fov_effects();
	
	printf("\n=== All camera ray tests passed! ===\n");
	return (0);
}