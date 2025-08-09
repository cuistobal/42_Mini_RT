/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing_comprehensive.c                       :+:      :+:    :+:   */
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
** Helper function to compare vectors with epsilon tolerance
*/
int	vec3_equals(t_vec3 a, t_vec3 b)
{
	return (double_equals(a.x, b.x) && 
			double_equals(a.y, b.y) && 
			double_equals(a.z, b.z));
}

/*
** Helper function to compare colors
*/
int	color_equals(t_color a, t_color b)
{
	return (a.r == b.r && a.g == b.g && a.b == b.b);
}

/*
** Test parse_double function
*/
void	test_parse_double(void)
{
	double	result;
	int		success;

	printf("Testing parse_double...\n");
	
	// Test positive number
	success = parse_double("42.5", &result);
	assert(success == 1);
	assert(double_equals(result, 42.5));
	
	// Test negative number
	success = parse_double("-3.14159", &result);
	assert(success == 1);
	assert(double_equals(result, -3.14159));
	
	// Test integer
	success = parse_double("100", &result);
	assert(success == 1);
	assert(double_equals(result, 100.0));
	
	// Test zero
	success = parse_double("0.0", &result);
	assert(success == 1);
	assert(double_equals(result, 0.0));
	
	// Test invalid input
	success = parse_double("not_a_number", &result);
	assert(success == 0);
	
	// Test empty string
	success = parse_double("", &result);
	assert(success == 0);
	
	// Test NULL input
	success = parse_double(NULL, &result);
	assert(success == 0);
	
	printf("✓ parse_double tests passed\n");
}

/*
** Test parse_int function
*/
void	test_parse_int(void)
{
	int		result;
	int		success;

	printf("Testing parse_int...\n");
	
	// Test positive integer
	success = parse_int("255", &result);
	assert(success == 1);
	assert(result == 255);
	
	// Test negative integer
	success = parse_int("-42", &result);
	assert(success == 1);
	assert(result == -42);
	
	// Test zero
	success = parse_int("0", &result);
	assert(success == 1);
	assert(result == 0);
	
	// Test invalid input (float)
	success = parse_int("3.14", &result);
	assert(success == 0);
	
	// Test invalid input (text)
	success = parse_int("abc", &result);
	assert(success == 0);
	
	// Test overflow (if applicable)
	success = parse_int("999999999999999999999", &result);
	assert(success == 0);
	
	printf("✓ parse_int tests passed\n");
}

/*
** Test parse_vec3 function
*/
void	test_parse_vec3(void)
{
	t_vec3	result;
	int		success;

	printf("Testing parse_vec3...\n");
	
	// Test basic vector
	success = parse_vec3("1.0,2.0,3.0", &result);
	assert(success == 1);
	assert(vec3_equals(result, vec3_new(1.0, 2.0, 3.0)));
	
	// Test negative values
	success = parse_vec3("-1.5,0.0,2.7", &result);
	assert(success == 1);
	assert(vec3_equals(result, vec3_new(-1.5, 0.0, 2.7)));
	
	// Test integers
	success = parse_vec3("0,1,2", &result);
	assert(success == 1);
	assert(vec3_equals(result, vec3_new(0.0, 1.0, 2.0)));
	
	// Test with spaces (should fail)
	success = parse_vec3("1.0, 2.0, 3.0", &result);
	assert(success == 0);
	
	// Test missing component
	success = parse_vec3("1.0,2.0", &result);
	assert(success == 0);
	
	// Test too many components
	success = parse_vec3("1.0,2.0,3.0,4.0", &result);
	assert(success == 0);
	
	// Test invalid component
	success = parse_vec3("1.0,abc,3.0", &result);
	assert(success == 0);
	
	printf("✓ parse_vec3 tests passed\n");
}

/*
** Test parse_color function
*/
void	test_parse_color(void)
{
	t_color	result;
	int		success;

	printf("Testing parse_color...\n");
	
	// Test basic color
	success = parse_color("255,128,0", &result);
	assert(success == 1);
	assert(color_equals(result, color_new(255, 128, 0)));
	
	// Test black
	success = parse_color("0,0,0", &result);
	assert(success == 1);
	assert(color_equals(result, color_new(0, 0, 0)));
	
	// Test white
	success = parse_color("255,255,255", &result);
	assert(success == 1);
	assert(color_equals(result, color_new(255, 255, 255)));
	
	// Test out of range (too high)
	success = parse_color("256,128,0", &result);
	assert(success == 0);
	
	// Test out of range (negative)
	success = parse_color("-1,128,0", &result);
	assert(success == 0);
	
	// Test missing component
	success = parse_color("255,128", &result);
	assert(success == 0);
	
	// Test invalid component
	success = parse_color("255,abc,0", &result);
	assert(success == 0);
	
	printf("✓ parse_color tests passed\n");
}/*

** Test get_next_token function
*/
void	test_get_next_token(void)
{
	char	*line;
	char	*token;

	printf("Testing get_next_token...\n");
	
	// Test basic tokenization
	line = safe_strdup("hello world test");
	token = get_next_token(&line);
	assert(strcmp(token, "hello") == 0);
	free(token);
	
	token = get_next_token(&line);
	assert(strcmp(token, "world") == 0);
	free(token);
	
	token = get_next_token(&line);
	assert(strcmp(token, "test") == 0);
	free(token);
	
	token = get_next_token(&line);
	assert(token == NULL);
	
	free(line - strlen("hello world test")); // Reset pointer for free
	
	// Test with multiple spaces
	line = safe_strdup("  token1    token2  ");
	token = get_next_token(&line);
	assert(strcmp(token, "token1") == 0);
	free(token);
	
	token = get_next_token(&line);
	assert(strcmp(token, "token2") == 0);
	free(token);
	
	free(line - strlen("  token1    token2  "));
	
	printf("✓ get_next_token tests passed\n");
}

/*
** Test scene element parsing
*/
void	test_scene_element_parsing(void)
{
	t_scene	scene;
	int		success;

	printf("Testing scene element parsing...\n");
	
	// Initialize scene
	scene.lights = NULL;
	scene.objects = NULL;
	scene.ambient_ratio = -1.0;
	scene.camera.fov = 0.0;
	
	// Test ambient lighting parsing
	success = parse_ambient("0.2 255,255,255", &scene);
	assert(success == 1);
	assert(double_equals(scene.ambient_ratio, 0.2));
	assert(color_equals(scene.ambient, color_new(255, 255, 255)));
	
	// Test camera parsing
	success = parse_camera("0,0,0 0,0,1 70", &scene);
	assert(success == 1);
	assert(vec3_equals(scene.camera.position, vec3_new(0, 0, 0)));
	assert(vec3_equals(scene.camera.direction, vec3_new(0, 0, 1)));
	assert(double_equals(scene.camera.fov, 70.0));
	
	// Test light parsing
	success = parse_light("0,10,0 0.7 255,255,255", &scene);
	assert(success == 1);
	assert(scene.lights != NULL);
	assert(vec3_equals(scene.lights->position, vec3_new(0, 10, 0)));
	assert(double_equals(scene.lights->intensity, 0.7));
	assert(color_equals(scene.lights->color, color_new(255, 255, 255)));
	
	// Cleanup
	cleanup_scene(&scene);
	
	printf("✓ Scene element parsing tests passed\n");
}

/*
** Test object parsing
*/
void	test_object_parsing(void)
{
	t_scene		scene;
	t_object	*obj;
	int			success;

	printf("Testing object parsing...\n");
	
	// Initialize scene
	scene.objects = NULL;
	
	// Test sphere parsing
	success = parse_sphere("0,0,10 5 255,0,0", &scene);
	assert(success == 1);
	assert(scene.objects != NULL);
	obj = scene.objects;
	assert(obj->type == SPHERE);
	assert(vec3_equals(obj->position, vec3_new(0, 0, 10)));
	assert(double_equals(obj->radius, 2.5)); // diameter/2
	assert(color_equals(obj->material.color, color_new(255, 0, 0)));
	
	// Test plane parsing
	success = parse_plane("0,-5,0 0,1,0 100,100,100", &scene);
	assert(success == 1);
	obj = scene.objects; // Should be the new plane (added to front)
	assert(obj->type == PLANE);
	assert(vec3_equals(obj->position, vec3_new(0, -5, 0)));
	assert(vec3_equals(obj->normal, vec3_new(0, 1, 0)));
	assert(color_equals(obj->material.color, color_new(100, 100, 100)));
	
	// Test cylinder parsing
	success = parse_cylinder("5,0,10 0,1,0 2 8 0,255,0", &scene);
	assert(success == 1);
	obj = scene.objects; // Should be the new cylinder
	assert(obj->type == CYLINDER);
	assert(vec3_equals(obj->position, vec3_new(5, 0, 10)));
	assert(vec3_equals(obj->axis, vec3_new(0, 1, 0)));
	assert(double_equals(obj->radius, 1.0)); // diameter/2
	assert(double_equals(obj->height, 8.0));
	assert(color_equals(obj->material.color, color_new(0, 255, 0)));
	
	// Test cone parsing
	success = parse_cone("3,0,5 0,1,0 30 6 255,255,0", &scene);
	assert(success == 1);
	obj = scene.objects; // Should be the new cone
	assert(obj->type == CONE);
	assert(vec3_equals(obj->position, vec3_new(3, 0, 5)));
	assert(vec3_equals(obj->axis, vec3_new(0, 1, 0)));
	assert(double_equals(obj->angle, 30.0));
	assert(double_equals(obj->height, 6.0));
	assert(color_equals(obj->material.color, color_new(255, 255, 0)));
	
	// Test cube parsing
	success = parse_cube("2,2,2 4 0,0,255", &scene);
	assert(success == 1);
	obj = scene.objects; // Should be the new cube
	assert(obj->type == CUBE);
	assert(vec3_equals(obj->position, vec3_new(2, 2, 2)));
	assert(double_equals(obj->radius, 2.0)); // size/2
	assert(color_equals(obj->material.color, color_new(0, 0, 255)));
	
	// Cleanup
	cleanup_scene(&scene);
	
	printf("✓ Object parsing tests passed\n");
}

/*
** Test parsing error cases
*/
void	test_parsing_error_cases(void)
{
	t_scene	scene;
	int		success;

	printf("Testing parsing error cases...\n");
	
	// Initialize scene
	scene.lights = NULL;
	scene.objects = NULL;
	scene.ambient_ratio = -1.0;
	scene.camera.fov = 0.0;
	
	// Test invalid ambient ratio (too high)
	success = parse_ambient("1.5 255,255,255", &scene);
	assert(success == 0);
	
	// Test invalid ambient ratio (negative)
	success = parse_ambient("-0.1 255,255,255", &scene);
	assert(success == 0);
	
	// Test invalid camera FOV (too high)
	success = parse_camera("0,0,0 0,0,1 200", &scene);
	assert(success == 0);
	
	// Test invalid camera FOV (zero)
	success = parse_camera("0,0,0 0,0,1 0", &scene);
	assert(success == 0);
	
	// Test invalid sphere diameter (negative)
	success = parse_sphere("0,0,10 -5 255,0,0", &scene);
	assert(success == 0);
	
	// Test invalid cylinder height (zero)
	success = parse_cylinder("5,0,10 0,1,0 2 0 0,255,0", &scene);
	assert(success == 0);
	
	// Test invalid cone angle (too high)
	success = parse_cone("3,0,5 0,1,0 95 6 255,255,0", &scene);
	assert(success == 0);
	
	// Test invalid cube size (negative)
	success = parse_cube("2,2,2 -4 0,0,255", &scene);
	assert(success == 0);
	
	// Test malformed input (missing parameters)
	success = parse_sphere("0,0,10 5", &scene);
	assert(success == 0);
	
	// Test malformed vector
	success = parse_sphere("0,0 5 255,0,0", &scene);
	assert(success == 0);
	
	// Test malformed color
	success = parse_sphere("0,0,10 5 255,0", &scene);
	assert(success == 0);
	
	printf("✓ Parsing error cases tests passed\n");
}

/*
** Test scene validation
*/
void	test_scene_validation(void)
{
	t_scene	scene;
	int		valid;

	printf("Testing scene validation...\n");
	
	// Test empty scene (should be invalid)
	scene.lights = NULL;
	scene.objects = NULL;
	scene.ambient_ratio = -1.0;
	scene.camera.fov = 0.0;
	valid = validate_scene(&scene);
	assert(valid == 0);
	
	// Test scene with only camera (should be invalid)
	scene.camera.fov = 70.0;
	valid = validate_scene(&scene);
	assert(valid == 0);
	
	// Test scene with camera and ambient (should be invalid - no lights)
	scene.ambient_ratio = 0.2;
	valid = validate_scene(&scene);
	assert(valid == 0);
	
	// Test complete valid scene
	parse_light("0,10,0 0.7 255,255,255", &scene);
	parse_sphere("0,0,10 5 255,0,0", &scene);
	valid = validate_scene(&scene);
	assert(valid == 1);
	
	// Cleanup
	cleanup_scene(&scene);
	
	printf("✓ Scene validation tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Comprehensive Parsing Unit Tests ===\n\n");
	
	test_parse_double();
	test_parse_int();
	test_parse_vec3();
	test_parse_color();
	test_get_next_token();
	test_scene_element_parsing();
	test_object_parsing();
	test_parsing_error_cases();
	test_scene_validation();
	
	printf("\n=== All parsing tests passed! ===\n");
	return (0);
}