/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_integration.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include <assert.h>
#include <time.h>

/*
** Test scene parsing and validation
*/
void	test_scene_parsing_integration(void)
{
	t_scene	scene;
	int		success;

	printf("Testing scene parsing integration...\n");
	
	// Test basic sphere scene
	success = parse_scene("tests/scenes/basic_sphere.rt", &scene);
	assert(success == 1);
	assert(scene.camera.fov > 0);
	assert(scene.ambient_ratio >= 0);
	assert(scene.lights != NULL);
	assert(scene.objects != NULL);
	assert(scene.objects->type == SPHERE);
	cleanup_scene(&scene);
	
	// Test multiple objects scene
	success = parse_scene("tests/scenes/multiple_objects.rt", &scene);
	assert(success == 1);
	
	// Count objects
	int object_count = 0;
	t_object *obj = scene.objects;
	while (obj)
	{
		object_count++;
		obj = obj->next;
	}
	assert(object_count == 4); // sphere, plane, cylinder, sphere
	
	// Count lights
	int light_count = 0;
	t_light *light = scene.lights;
	while (light)
	{
		light_count++;
		light = light->next;
	}
	assert(light_count == 2); // Two lights
	
	cleanup_scene(&scene);
	
	// Test bonus objects scene
	success = parse_scene("tests/scenes/bonus_objects.rt", &scene);
	assert(success == 1);
	
	// Verify bonus objects are parsed
	int has_cone = 0, has_cube = 0;
	obj = scene.objects;
	while (obj)
	{
		if (obj->type == CONE)
			has_cone = 1;
		if (obj->type == CUBE)
			has_cube = 1;
		obj = obj->next;
	}
	assert(has_cone == 1);
	assert(has_cube == 1);
	
	cleanup_scene(&scene);
	
	printf("✓ Scene parsing integration tests passed\n");
}

/*
** Test complete rendering pipeline
*/
void	test_rendering_pipeline(void)
{
	t_scene		scene;
	t_camera	*camera;
	t_ray		ray;
	t_color		pixel_color;
	t_hit		hit;
	int			success;

	printf("Testing rendering pipeline integration...\n");
	
	// Load test scene
	success = parse_scene("tests/scenes/basic_sphere.rt", &scene);
	assert(success == 1);
	
	camera = &scene.camera;
	setup_camera(camera);
	
	// Test camera ray generation
	ray = get_camera_ray(camera, 0.5, 0.5); // Center pixel
	assert(ray.origin.x == camera->position.x);
	assert(ray.origin.y == camera->position.y);
	assert(ray.origin.z == camera->position.z);
	
	// Test intersection
	success = intersect_scene(ray, &scene, &hit);
	assert(success == 1); // Should hit the sphere
	assert(hit.object->type == SPHERE);
	
	// Test lighting calculation
	pixel_color = calculate_lighting(hit.point, hit.normal, &scene, hit.material);
	assert(pixel_color.r >= 0 && pixel_color.r <= 255);
	assert(pixel_color.g >= 0 && pixel_color.g <= 255);
	assert(pixel_color.b >= 0 && pixel_color.b <= 255);
	
	// Test complete raycast
	pixel_color = raycast(ray, &scene, MAX_DEPTH);
	assert(pixel_color.r >= 0 && pixel_color.r <= 255);
	assert(pixel_color.g >= 0 && pixel_color.g <= 255);
	assert(pixel_color.b >= 0 && pixel_color.b <= 255);
	
	cleanup_scene(&scene);
	
	printf("✓ Rendering pipeline integration tests passed\n");
}

/*
** Test lighting scenarios
*/
void	test_lighting_scenarios(void)
{
	t_scene		scene;
	t_ray		ray;
	t_color		pixel_color;
	t_hit		hit;
	int			success;

	printf("Testing lighting scenarios...\n");
	
	// Load scene with multiple lights
	success = parse_scene("tests/scenes/multiple_objects.rt", &scene);
	assert(success == 1);
	
	setup_camera(&scene.camera);
	
	// Test ray hitting sphere (should be lit by multiple lights)
	ray = get_camera_ray(&scene.camera, 0.5, 0.5);
	success = intersect_scene(ray, &scene, &hit);
	if (success)
	{
		pixel_color = calculate_lighting(hit.point, hit.normal, &scene, hit.material);
		
		// Color should be influenced by lighting
		assert(pixel_color.r > 0 || pixel_color.g > 0 || pixel_color.b > 0);
	}
	
	// Test shadow calculation
	if (success && scene.lights && scene.lights->next)
	{
		t_light *light = scene.lights;
		int in_shadow = is_in_shadow(hit.point, light, &scene);
		// Shadow result should be valid (0 or 1)
		assert(in_shadow == 0 || in_shadow == 1);
	}
	
	cleanup_scene(&scene);
	
	printf("✓ Lighting scenarios tests passed\n");
}

/*
** Test all object types intersection
*/
void	test_all_object_types(void)
{
	t_scene		scene;
	t_ray		ray;
	t_hit		hit;
	int			success;
	int			sphere_hit = 0, plane_hit = 0, cylinder_hit = 0;
	int			cone_hit = 0, cube_hit = 0;

	printf("Testing all object types intersection...\n");
	
	// Load scene with all object types
	success = parse_scene("tests/scenes/bonus_objects.rt", &scene);
	assert(success == 1);
	
	setup_camera(&scene.camera);
	
	// Test multiple rays to hit different objects
	for (int i = 0; i < 100; i++)
	{
		double u = (double)i / 100.0;
		double v = 0.5;
		
		ray = get_camera_ray(&scene.camera, u, v);
		success = intersect_scene(ray, &scene, &hit);
		
		if (success)
		{
			switch (hit.object->type)
			{
				case SPHERE:
					sphere_hit = 1;
					break;
				case PLANE:
					plane_hit = 1;
					break;
				case CYLINDER:
					cylinder_hit = 1;
					break;
				case CONE:
					cone_hit = 1;
					break;
				case CUBE:
					cube_hit = 1;
					break;
			}
		}
	}
	
	// Should have hit at least some object types
	assert(sphere_hit || plane_hit || cylinder_hit || cone_hit || cube_hit);
	
	cleanup_scene(&scene);
	
	printf("✓ All object types intersection tests passed\n");
}

/*
** Test performance with complex scene
*/
void	test_performance_complex_scene(void)
{
	t_scene		scene;
	t_ray		ray;
	t_color		pixel_color;
	clock_t		start, end;
	double		cpu_time_used;
	int			success;
	int			pixel_count = 0;

	printf("Testing performance with complex scene...\n");
	
	// Load complex scene
	success = parse_scene("tests/scenes/bonus_objects.rt", &scene);
	assert(success == 1);
	
	setup_camera(&scene.camera);
	
	start = clock();
	
	// Render a small portion of the image (10x10 pixels)
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			double u = (double)x / 10.0;
			double v = (double)y / 10.0;
			
			ray = get_camera_ray(&scene.camera, u, v);
			pixel_color = raycast(ray, &scene, MAX_DEPTH);
			
			// Verify color is valid
			assert(pixel_color.r >= 0 && pixel_color.r <= 255);
			assert(pixel_color.g >= 0 && pixel_color.g <= 255);
			assert(pixel_color.b >= 0 && pixel_color.b <= 255);
			
			pixel_count++;
		}
	}
	
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	
	printf("Rendered %d pixels in %f seconds\n", pixel_count, cpu_time_used);
	printf("Average time per pixel: %f ms\n", (cpu_time_used * 1000.0) / pixel_count);
	
	// Performance should be reasonable (less than 10ms per pixel)
	assert((cpu_time_used / pixel_count) < 0.01);
	
	cleanup_scene(&scene);
	
	printf("✓ Performance tests passed\n");
}

/*
** Test memory usage and cleanup
*/
void	test_memory_management(void)
{
	t_scene	scene;
	int		success;

	printf("Testing memory management...\n");
	
	// Test multiple scene loads and cleanups
	for (int i = 0; i < 10; i++)
	{
		success = parse_scene("tests/scenes/multiple_objects.rt", &scene);
		assert(success == 1);
		
		// Verify scene is properly loaded
		assert(scene.objects != NULL);
		assert(scene.lights != NULL);
		
		// Cleanup should not crash
		cleanup_scene(&scene);
		
		// After cleanup, pointers should be NULL
		assert(scene.objects == NULL);
		assert(scene.lights == NULL);
	}
	
	printf("✓ Memory management tests passed\n");
}

/*
** Test error handling in integration
*/
void	test_error_handling_integration(void)
{
	t_scene	scene;
	int		success;

	printf("Testing error handling integration...\n");
	
	// Test non-existent file
	success = parse_scene("non_existent_file.rt", &scene);
	assert(success == 0);
	
	// Test invalid scene file
	FILE *invalid_file = fopen("test_invalid_temp.rt", "w");
	fprintf(invalid_file, "INVALID CONTENT\n");
	fclose(invalid_file);
	
	success = parse_scene("test_invalid_temp.rt", &scene);
	assert(success == 0);
	
	// Cleanup test file
	remove("test_invalid_temp.rt");
	
	printf("✓ Error handling integration tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Integration Tests ===\n\n");
	
	test_scene_parsing_integration();
	test_rendering_pipeline();
	test_lighting_scenarios();
	test_all_object_types();
	test_performance_complex_scene();
	test_memory_management();
	test_error_handling_integration();
	
	printf("\n=== All integration tests passed! ===\n");
	return (0);
}