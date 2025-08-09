/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector_raytracing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"
#include <assert.h>

#define TEST_EPSILON 1e-9

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
** Test ray tracing specific vector operations
*/
void	test_raytracing_scenarios(void)
{
	t_vec3	ray_origin, ray_direction, sphere_center, to_sphere;
	t_vec3	surface_normal, reflected_dir, light_dir;
	double	distance, dot_product;

	printf("Testing ray tracing specific scenarios...\n");
	
	// Test 1: Ray to sphere center calculation
	ray_origin = vec3_new(0.0, 0.0, 0.0);
	sphere_center = vec3_new(0.0, 0.0, 10.0);
	to_sphere = vec3_sub(sphere_center, ray_origin);
	distance = vec3_length(to_sphere);
	assert(double_equals(distance, 10.0));
	
	// Test 2: Surface normal calculation (normalized vector from center to point)
	t_vec3 sphere_point = vec3_new(1.0, 0.0, 10.0);
	surface_normal = vec3_normalize(vec3_sub(sphere_point, sphere_center));
	t_vec3 expected_normal = vec3_new(1.0, 0.0, 0.0);
	assert(vec3_equals(surface_normal, expected_normal));
	
	// Test 3: Light direction calculation
	t_vec3 light_pos = vec3_new(5.0, 5.0, 5.0);
	t_vec3 surface_point = vec3_new(1.0, 0.0, 10.0);
	light_dir = vec3_normalize(vec3_sub(light_pos, surface_point));
	
	// Test 4: Diffuse lighting calculation (dot product)
	dot_product = vec3_dot(surface_normal, light_dir);
	assert(dot_product >= 0.0 && dot_product <= 1.0);
	
	// Test 5: Reflection calculation
	// R = D - 2(D·N)N where D is incident direction, N is normal
	ray_direction = vec3_new(-1.0, -1.0, 0.0);
	ray_direction = vec3_normalize(ray_direction);
	surface_normal = vec3_new(0.0, 1.0, 0.0);
	
	double dot_dn = vec3_dot(ray_direction, surface_normal);
	t_vec3 two_dot_n = vec3_mult(surface_normal, 2.0 * dot_dn);
	reflected_dir = vec3_sub(ray_direction, two_dot_n);
	
	// Reflected ray should have same angle with normal but opposite side
	double incident_angle = acos(fabs(vec3_dot(ray_direction, surface_normal)));
	double reflected_angle = acos(fabs(vec3_dot(reflected_dir, surface_normal)));
	assert(double_equals(incident_angle, reflected_angle));
	
	printf("✓ Ray tracing scenarios tests passed\n");
}

/*
** Test camera coordinate system setup
*/
void	test_camera_vectors(void)
{
	t_vec3	camera_dir, camera_up, camera_right;
	t_vec3	world_up;

	printf("Testing camera coordinate system...\n");
	
	// Standard camera looking down Z axis
	camera_dir = vec3_new(0.0, 0.0, 1.0);
	world_up = vec3_new(0.0, 1.0, 0.0);
	
	// Right vector = direction × up
	camera_right = vec3_normalize(vec3_cross(camera_dir, world_up));
	t_vec3 expected_right = vec3_new(-1.0, 0.0, 0.0);
	assert(vec3_equals(camera_right, expected_right));
	
	// Up vector = right × direction
	camera_up = vec3_normalize(vec3_cross(camera_right, camera_dir));
	t_vec3 expected_up = vec3_new(0.0, 1.0, 0.0);
	assert(vec3_equals(camera_up, expected_up));
	
	// All vectors should be unit length
	assert(double_equals(vec3_length(camera_dir), 1.0));
	assert(double_equals(vec3_length(camera_up), 1.0));
	assert(double_equals(vec3_length(camera_right), 1.0));
	
	// All vectors should be orthogonal
	assert(double_equals(vec3_dot(camera_dir, camera_up), 0.0));
	assert(double_equals(vec3_dot(camera_dir, camera_right), 0.0));
	assert(double_equals(vec3_dot(camera_up, camera_right), 0.0));
	
	printf("✓ Camera coordinate system tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Ray Tracing Vector Operations Tests ===\n\n");
	
	test_raytracing_scenarios();
	test_camera_vectors();
	
	printf("\n=== All ray tracing vector tests passed! ===\n");
	return (0);
}