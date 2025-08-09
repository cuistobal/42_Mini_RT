/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ray.c                                         :+:      :+:    :+:   */
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
** Test ray_new function
*/
void	test_ray_new(void)
{
	t_ray	ray;
	t_vec3	origin, direction, expected_dir;

	printf("Testing ray_new...\n");
	
	// Test with normalized direction
	origin = vec3_new(0.0, 0.0, 0.0);
	direction = vec3_new(1.0, 0.0, 0.0);
	ray = ray_new(origin, direction);
	
	assert(vec3_equals(ray.origin, origin));
	expected_dir = vec3_new(1.0, 0.0, 0.0);
	assert(vec3_equals(ray.direction, expected_dir));
	
	// Test with non-normalized direction (should be normalized)
	direction = vec3_new(3.0, 4.0, 0.0); // Length = 5
	ray = ray_new(origin, direction);
	expected_dir = vec3_new(0.6, 0.8, 0.0); // Normalized
	assert(vec3_equals(ray.direction, expected_dir));
	
	// Check that direction is unit length
	double length = vec3_length(ray.direction);
	assert(double_equals(length, 1.0));
	
	printf("✓ ray_new tests passed\n");
}

/*
** Test ray_at function
*/
void	test_ray_at(void)
{
	t_ray	ray;
	t_vec3	point, expected;

	printf("Testing ray_at...\n");
	
	// Test basic ray point calculation
	ray.origin = vec3_new(1.0, 2.0, 3.0);
	ray.direction = vec3_new(1.0, 0.0, 0.0);
	
	// At t=0, should return origin
	point = ray_at(ray, 0.0);
	expected = vec3_new(1.0, 2.0, 3.0);
	assert(vec3_equals(point, expected));
	
	// At t=5, should be origin + 5*direction
	point = ray_at(ray, 5.0);
	expected = vec3_new(6.0, 2.0, 3.0);
	assert(vec3_equals(point, expected));
	
	// Test with negative t
	point = ray_at(ray, -2.0);
	expected = vec3_new(-1.0, 2.0, 3.0);
	assert(vec3_equals(point, expected));
	
	// Test with diagonal direction
	ray.direction = vec3_normalize(vec3_new(1.0, 1.0, 1.0));
	point = ray_at(ray, sqrt(3.0)); // Distance to move 1 unit in each direction
	expected = vec3_new(2.0, 3.0, 4.0);
	assert(vec3_equals(point, expected));
	
	printf("✓ ray_at tests passed\n");
}

/*
** Test ray_transform function
*/
void	test_ray_transform(void)
{
	t_ray	ray, transformed;
	t_vec3	translation, expected_origin;

	printf("Testing ray_transform...\n");
	
	// Test basic translation
	ray.origin = vec3_new(1.0, 2.0, 3.0);
	ray.direction = vec3_new(1.0, 0.0, 0.0);
	translation = vec3_new(5.0, -2.0, 1.0);
	
	transformed = ray_transform(ray, translation);
	
	expected_origin = vec3_new(6.0, 0.0, 4.0);
	assert(vec3_equals(transformed.origin, expected_origin));
	
	// Direction should remain unchanged
	assert(vec3_equals(transformed.direction, ray.direction));
	
	// Test zero translation
	translation = vec3_new(0.0, 0.0, 0.0);
	transformed = ray_transform(ray, translation);
	assert(vec3_equals(transformed.origin, ray.origin));
	assert(vec3_equals(transformed.direction, ray.direction));
	
	printf("✓ ray_transform tests passed\n");
}/*

** Test ray_reflect function
*/
void	test_ray_reflect(void)
{
	t_ray	incident, reflected;
	t_vec3	hit_point, normal, expected_dir;

	printf("Testing ray_reflect...\n");
	
	// Test reflection off horizontal surface (floor)
	incident.origin = vec3_new(0.0, 5.0, 0.0);
	incident.direction = vec3_normalize(vec3_new(1.0, -1.0, 0.0)); // 45° down
	hit_point = vec3_new(1.0, 0.0, 0.0);
	normal = vec3_new(0.0, 1.0, 0.0); // Up normal
	
	reflected = ray_reflect(incident, hit_point, normal);
	
	// Origin should be at hit point
	assert(vec3_equals(reflected.origin, hit_point));
	
	// Direction should be reflected (45° up)
	expected_dir = vec3_normalize(vec3_new(1.0, 1.0, 0.0));
	assert(vec3_equals(reflected.direction, expected_dir));
	
	// Test reflection off vertical surface (wall)
	incident.direction = vec3_normalize(vec3_new(1.0, 0.0, 0.0)); // Right
	normal = vec3_new(-1.0, 0.0, 0.0); // Left normal
	
	reflected = ray_reflect(incident, hit_point, normal);
	expected_dir = vec3_normalize(vec3_new(-1.0, 0.0, 0.0)); // Left
	assert(vec3_equals(reflected.direction, expected_dir));
	
	// Test perfect perpendicular reflection
	incident.direction = vec3_new(0.0, -1.0, 0.0); // Straight down
	normal = vec3_new(0.0, 1.0, 0.0); // Up normal
	
	reflected = ray_reflect(incident, hit_point, normal);
	expected_dir = vec3_new(0.0, 1.0, 0.0); // Straight up
	assert(vec3_equals(reflected.direction, expected_dir));
	
	printf("✓ ray_reflect tests passed\n");
}

/*
** Test ray_distance_to_point function
*/
void	test_ray_distance_to_point(void)
{
	t_ray	ray;
	t_vec3	point;
	double	distance;

	printf("Testing ray_distance_to_point...\n");
	
	// Test basic distance calculation
	ray.origin = vec3_new(0.0, 0.0, 0.0);
	ray.direction = vec3_new(1.0, 0.0, 0.0);
	
	// Point at origin should have distance 0
	point = vec3_new(0.0, 0.0, 0.0);
	distance = ray_distance_to_point(ray, point);
	assert(double_equals(distance, 0.0));
	
	// Point along positive direction
	point = vec3_new(5.0, 0.0, 0.0);
	distance = ray_distance_to_point(ray, point);
	assert(double_equals(distance, 5.0));
	
	// Point behind ray origin (negative distance)
	point = vec3_new(-3.0, 0.0, 0.0);
	distance = ray_distance_to_point(ray, point);
	assert(double_equals(distance, -3.0));
	
	// Test with diagonal ray
	ray.direction = vec3_normalize(vec3_new(1.0, 1.0, 0.0));
	point = vec3_new(1.0, 1.0, 0.0);
	distance = ray_distance_to_point(ray, point);
	assert(double_equals(distance, sqrt(2.0)));
	
	// Test point not on ray line (should project onto ray)
	ray.direction = vec3_new(1.0, 0.0, 0.0);
	point = vec3_new(3.0, 5.0, 2.0); // Off to the side
	distance = ray_distance_to_point(ray, point);
	assert(double_equals(distance, 3.0)); // Only X component matters
	
	printf("✓ ray_distance_to_point tests passed\n");
}

/*
** Test ray tracing specific scenarios
*/
void	test_ray_tracing_scenarios(void)
{
	t_ray	camera_ray, shadow_ray;
	t_vec3	pixel_pos, light_pos, surface_point;

	printf("Testing ray tracing specific scenarios...\n");
	
	// Test camera ray generation (simplified)
	t_vec3 camera_pos = vec3_new(0.0, 0.0, 0.0);
	pixel_pos = vec3_new(1.0, 1.0, 2.0); // Screen pixel position
	t_vec3 ray_dir = vec3_sub(pixel_pos, camera_pos);
	camera_ray = ray_new(camera_pos, ray_dir);
	
	// Ray should start at camera and point toward pixel
	assert(vec3_equals(camera_ray.origin, camera_pos));
	assert(double_equals(vec3_length(camera_ray.direction), 1.0));
	
	// Test shadow ray generation
	surface_point = vec3_new(2.0, 0.0, 1.0);
	light_pos = vec3_new(5.0, 3.0, 2.0);
	t_vec3 to_light = vec3_sub(light_pos, surface_point);
	shadow_ray = ray_new(surface_point, to_light);
	
	// Shadow ray should start at surface and point toward light
	assert(vec3_equals(shadow_ray.origin, surface_point));
	
	// Distance to light should be calculable
	double light_distance = vec3_length(to_light);
	t_vec3 light_point = ray_at(shadow_ray, light_distance);
	assert(vec3_equals(light_point, light_pos));
	
	printf("✓ Ray tracing scenarios tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Ray Operations Unit Tests ===\n\n");
	
	test_ray_new();
	test_ray_at();
	test_ray_transform();
	test_ray_reflect();
	test_ray_distance_to_point();
	test_ray_tracing_scenarios();
	
	printf("\n=== All ray tests passed! ===\n");
	return (0);
}