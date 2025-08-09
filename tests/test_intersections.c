/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
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
** Helper function to create a test sphere
*/
t_object	create_test_sphere(t_vec3 center, double radius, t_color color)
{
	t_object	sphere;

	sphere.type = SPHERE;
	sphere.position = center;
	sphere.radius = radius;
	sphere.material.color = color;
	sphere.material.reflection = 0.0;
	sphere.material.transparency = 0.0;
	sphere.material.refraction_index = 1.0;
	sphere.next = NULL;
	return (sphere);
}

/*
** Helper function to create a test plane
*/
t_object	create_test_plane(t_vec3 point, t_vec3 normal, t_color color)
{
	t_object	plane;

	plane.type = PLANE;
	plane.position = point;
	plane.normal = vec3_normalize(normal);
	plane.material.color = color;
	plane.material.reflection = 0.0;
	plane.material.transparency = 0.0;
	plane.material.refraction_index = 1.0;
	plane.next = NULL;
	return (plane);
}

/*
** Helper function to create a test cylinder
*/
t_object	create_test_cylinder(t_vec3 center, t_vec3 axis, double radius, double height, t_color color)
{
	t_object	cylinder;

	cylinder.type = CYLINDER;
	cylinder.position = center;
	cylinder.axis = vec3_normalize(axis);
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.material.color = color;
	cylinder.material.reflection = 0.0;
	cylinder.material.transparency = 0.0;
	cylinder.material.refraction_index = 1.0;
	cylinder.next = NULL;
	return (cylinder);
}

/*
** Test sphere intersection - basic cases
*/
void	test_sphere_intersection_basic(void)
{
	t_object	sphere;
	t_ray		ray;
	double		t;

	printf("Testing sphere intersection - basic cases...\n");
	
	// Create sphere at origin with radius 1
	sphere = create_test_sphere(vec3_new(0, 0, 0), 1.0, color_new(255, 0, 0));
	
	// Ray from outside hitting sphere
	ray = ray_new(vec3_new(-3, 0, 0), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(double_equals(t, 2.0)); // Should hit at (-1, 0, 0)
	
	// Ray from inside sphere
	ray = ray_new(vec3_new(0, 0, 0), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(double_equals(t, 1.0)); // Should hit at (1, 0, 0)
	
	// Ray missing sphere
	ray = ray_new(vec3_new(-3, 2, 0), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(t < 0); // No intersection
	
	// Ray tangent to sphere
	ray = ray_new(vec3_new(-3, 1, 0), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(double_equals(t, 3.0)); // Should just touch at (0, 1, 0)
	
	printf("✓ Sphere intersection basic tests passed\n");
}

/*
** Test sphere intersection - edge cases
*/
void	test_sphere_intersection_edge_cases(void)
{
	t_object	sphere;
	t_ray		ray;
	double		t;

	printf("Testing sphere intersection - edge cases...\n");
	
	// Sphere not at origin
	sphere = create_test_sphere(vec3_new(5, 3, -2), 2.0, color_new(0, 255, 0));
	
	// Ray hitting moved sphere
	ray = ray_new(vec3_new(0, 3, -2), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(double_equals(t, 3.0)); // Should hit at (3, 3, -2)
	
	// Very small sphere
	sphere = create_test_sphere(vec3_new(0, 0, 0), 0.001, color_new(0, 0, 255));
	ray = ray_new(vec3_new(-1, 0, 0), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(double_equals(t, 0.999)); // Should hit tiny sphere
	
	// Ray pointing away from sphere
	sphere = create_test_sphere(vec3_new(0, 0, 0), 1.0, color_new(255, 255, 0));
	ray = ray_new(vec3_new(-3, 0, 0), vec3_new(-1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	assert(t < 0); // No intersection (pointing away)
	
	printf("✓ Sphere intersection edge cases tests passed\n");
}

/*
** Test plane intersection - basic cases
*/
void	test_plane_intersection_basic(void)
{
	t_object	plane;
	t_ray		ray;
	double		t;

	printf("Testing plane intersection - basic cases...\n");
	
	// Horizontal plane at y=0
	plane = create_test_plane(vec3_new(0, 0, 0), vec3_new(0, 1, 0), color_new(128, 128, 128));
	
	// Ray from above hitting plane
	ray = ray_new(vec3_new(0, 5, 0), vec3_new(0, -1, 0));
	t = intersect_plane(ray, &plane);
	assert(double_equals(t, 5.0)); // Should hit at (0, 0, 0)
	
	// Ray from below hitting plane
	ray = ray_new(vec3_new(0, -3, 0), vec3_new(0, 1, 0));
	t = intersect_plane(ray, &plane);
	assert(double_equals(t, 3.0)); // Should hit at (0, 0, 0)
	
	// Ray parallel to plane
	ray = ray_new(vec3_new(0, 1, 0), vec3_new(1, 0, 0));
	t = intersect_plane(ray, &plane);
	assert(t < 0); // No intersection (parallel)
	
	// Ray in plane
	ray = ray_new(vec3_new(0, 0, 0), vec3_new(1, 0, 0));
	t = intersect_plane(ray, &plane);
	assert(t < 0); // No intersection (in plane)
	
	printf("✓ Plane intersection basic tests passed\n");
}

/*
** Test plane intersection - angled planes
*/
void	test_plane_intersection_angled(void)
{
	t_object	plane;
	t_ray		ray;
	double		t;

	printf("Testing plane intersection - angled planes...\n");
	
	// 45-degree angled plane
	plane = create_test_plane(vec3_new(0, 0, 0), vec3_new(1, 1, 0), color_new(255, 128, 0));
	
	// Ray hitting angled plane
	ray = ray_new(vec3_new(-2, 2, 0), vec3_new(1, -1, 0));
	t = intersect_plane(ray, &plane);
	assert(t > 0); // Should intersect
	
	// Vertical plane (wall)
	plane = create_test_plane(vec3_new(5, 0, 0), vec3_new(1, 0, 0), color_new(0, 255, 255));
	ray = ray_new(vec3_new(0, 0, 0), vec3_new(1, 0, 0));
	t = intersect_plane(ray, &plane);
	assert(double_equals(t, 5.0)); // Should hit at (5, 0, 0)
	
	printf("✓ Plane intersection angled tests passed\n");
}/*

** Test cylinder intersection - basic cases
*/
void	test_cylinder_intersection_basic(void)
{
	t_object	cylinder;
	t_ray		ray;
	double		t;

	printf("Testing cylinder intersection - basic cases...\n");
	
	// Vertical cylinder at origin
	cylinder = create_test_cylinder(vec3_new(0, 0, 0), vec3_new(0, 1, 0), 1.0, 2.0, color_new(255, 0, 255));
	
	// Ray hitting cylinder from side
	ray = ray_new(vec3_new(-3, 1, 0), vec3_new(1, 0, 0));
	t = intersect_cylinder(ray, &cylinder);
	assert(t > 0); // Should intersect
	assert(double_equals(t, 2.0)); // Should hit at (-1, 1, 0)
	
	// Ray missing cylinder (too high)
	ray = ray_new(vec3_new(-3, 3, 0), vec3_new(1, 0, 0));
	t = intersect_cylinder(ray, &cylinder);
	assert(t < 0); // No intersection (above cylinder)
	
	// Ray missing cylinder (too low)
	ray = ray_new(vec3_new(-3, -1, 0), vec3_new(1, 0, 0));
	t = intersect_cylinder(ray, &cylinder);
	assert(t < 0); // No intersection (below cylinder)
	
	// Ray along cylinder axis
	ray = ray_new(vec3_new(0, -1, 0), vec3_new(0, 1, 0));
	t = intersect_cylinder(ray, &cylinder);
	assert(t < 0); // No intersection (inside cylinder)
	
	printf("✓ Cylinder intersection basic tests passed\n");
}

/*
** Test intersection scene function
*/
void	test_intersect_scene(void)
{
	t_scene		scene;
	t_object	sphere1, sphere2, plane;
	t_ray		ray;
	t_hit		hit;
	int			result;

	printf("Testing intersect_scene function...\n");
	
	// Create test scene with multiple objects
	sphere1 = create_test_sphere(vec3_new(0, 0, 5), 1.0, color_new(255, 0, 0));
	sphere2 = create_test_sphere(vec3_new(0, 0, 10), 1.0, color_new(0, 255, 0));
	plane = create_test_plane(vec3_new(0, -2, 0), vec3_new(0, 1, 0), color_new(128, 128, 128));
	
	// Link objects
	sphere1.next = &sphere2;
	sphere2.next = &plane;
	plane.next = NULL;
	scene.objects = &sphere1;
	
	// Ray hitting closest sphere
	ray = ray_new(vec3_new(0, 0, 0), vec3_new(0, 0, 1));
	result = intersect_scene(ray, &scene, &hit);
	assert(result == 1); // Should find intersection
	assert(double_equals(hit.t, 4.0)); // Distance to first sphere
	assert(hit.object == &sphere1); // Should hit first sphere
	
	// Ray hitting plane (closest)
	ray = ray_new(vec3_new(0, 5, 0), vec3_new(0, -1, 0));
	result = intersect_scene(ray, &scene, &hit);
	assert(result == 1); // Should find intersection
	assert(double_equals(hit.t, 7.0)); // Distance to plane
	assert(hit.object == &plane); // Should hit plane
	
	// Ray missing all objects
	ray = ray_new(vec3_new(10, 10, 0), vec3_new(1, 0, 0));
	result = intersect_scene(ray, &scene, &hit);
	assert(result == 0); // Should find no intersection
	
	printf("✓ Intersect scene tests passed\n");
}

/*
** Test intersection accuracy with known results
*/
void	test_intersection_accuracy(void)
{
	t_object	sphere;
	t_ray		ray;
	double		t;
	t_vec3		hit_point, expected_point;

	printf("Testing intersection accuracy with known results...\n");
	
	// Unit sphere at origin - test precise intersection points
	sphere = create_test_sphere(vec3_new(0, 0, 0), 1.0, color_new(255, 255, 255));
	
	// Ray along X-axis
	ray = ray_new(vec3_new(-2, 0, 0), vec3_new(1, 0, 0));
	t = intersect_sphere(ray, &sphere);
	hit_point = ray_at(ray, t);
	expected_point = vec3_new(-1, 0, 0);
	assert(double_equals(hit_point.x, expected_point.x));
	assert(double_equals(hit_point.y, expected_point.y));
	assert(double_equals(hit_point.z, expected_point.z));
	
	// Ray along Y-axis
	ray = ray_new(vec3_new(0, -2, 0), vec3_new(0, 1, 0));
	t = intersect_sphere(ray, &sphere);
	hit_point = ray_at(ray, t);
	expected_point = vec3_new(0, -1, 0);
	assert(double_equals(hit_point.x, expected_point.x));
	assert(double_equals(hit_point.y, expected_point.y));
	assert(double_equals(hit_point.z, expected_point.z));
	
	// Ray along Z-axis
	ray = ray_new(vec3_new(0, 0, -2), vec3_new(0, 0, 1));
	t = intersect_sphere(ray, &sphere);
	hit_point = ray_at(ray, t);
	expected_point = vec3_new(0, 0, -1);
	assert(double_equals(hit_point.x, expected_point.x));
	assert(double_equals(hit_point.y, expected_point.y));
	assert(double_equals(hit_point.z, expected_point.z));
	
	// Diagonal ray
	ray = ray_new(vec3_new(-2, -2, 0), vec3_normalize(vec3_new(1, 1, 0)));
	t = intersect_sphere(ray, &sphere);
	hit_point = ray_at(ray, t);
	// Hit point should be on unit circle
	double distance_from_origin = vec3_length(hit_point);
	assert(double_equals(distance_from_origin, 1.0));
	
	printf("✓ Intersection accuracy tests passed\n");
}

/*
** Test performance with many intersections
*/
void	test_intersection_performance(void)
{
	t_object	spheres[100];
	t_scene		scene;
	t_ray		ray;
	t_hit		hit;
	int			i;
	clock_t		start, end;
	double		cpu_time_used;

	printf("Testing intersection performance...\n");
	
	// Create many spheres
	for (i = 0; i < 100; i++)
	{
		spheres[i] = create_test_sphere(
			vec3_new(i * 2.0, 0, 10 + i), 
			1.0, 
			color_new(i % 256, (i * 2) % 256, (i * 3) % 256)
		);
		if (i < 99)
			spheres[i].next = &spheres[i + 1];
		else
			spheres[i].next = NULL;
	}
	scene.objects = &spheres[0];
	
	// Test many ray intersections
	ray = ray_new(vec3_new(0, 0, 0), vec3_new(0, 0, 1));
	
	start = clock();
	for (i = 0; i < 1000; i++)
	{
		intersect_scene(ray, &scene, &hit);
	}
	end = clock();
	
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("1000 intersection tests with 100 spheres took %f seconds\n", cpu_time_used);
	
	// Should complete in reasonable time (less than 1 second)
	assert(cpu_time_used < 1.0);
	
	printf("✓ Intersection performance tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Intersection Algorithm Unit Tests ===\n\n");
	
	test_sphere_intersection_basic();
	test_sphere_intersection_edge_cases();
	test_plane_intersection_basic();
	test_plane_intersection_angled();
	test_cylinder_intersection_basic();
	test_intersect_scene();
	test_intersection_accuracy();
	test_intersection_performance();
	
	printf("\n=== All intersection tests passed! ===\n");
	return (0);
}