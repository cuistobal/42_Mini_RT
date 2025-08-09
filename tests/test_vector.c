/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector.c                                      :+:      :+:    :+:   */
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
** Test vec3_new function
*/
void	test_vec3_new(void)
{
	t_vec3	v;

	printf("Testing vec3_new...\n");
	
	v = vec3_new(1.0, 2.0, 3.0);
	assert(double_equals(v.x, 1.0));
	assert(double_equals(v.y, 2.0));
	assert(double_equals(v.z, 3.0));
	
	v = vec3_new(-1.5, 0.0, 42.7);
	assert(double_equals(v.x, -1.5));
	assert(double_equals(v.y, 0.0));
	assert(double_equals(v.z, 42.7));
	
	printf("✓ vec3_new tests passed\n");
}

/*
** Test vec3_add function
*/
void	test_vec3_add(void)
{
	t_vec3	a, b, result, expected;

	printf("Testing vec3_add...\n");
	
	a = vec3_new(1.0, 2.0, 3.0);
	b = vec3_new(4.0, 5.0, 6.0);
	result = vec3_add(a, b);
	expected = vec3_new(5.0, 7.0, 9.0);
	assert(vec3_equals(result, expected));
	
	a = vec3_new(-1.0, 0.0, 1.0);
	b = vec3_new(1.0, -2.0, 3.0);
	result = vec3_add(a, b);
	expected = vec3_new(0.0, -2.0, 4.0);
	assert(vec3_equals(result, expected));
	
	printf("✓ vec3_add tests passed\n");
}

/*
** Test vec3_sub function
*/
void	test_vec3_sub(void)
{
	t_vec3	a, b, result, expected;

	printf("Testing vec3_sub...\n");
	
	a = vec3_new(5.0, 7.0, 9.0);
	b = vec3_new(1.0, 2.0, 3.0);
	result = vec3_sub(a, b);
	expected = vec3_new(4.0, 5.0, 6.0);
	assert(vec3_equals(result, expected));
	
	a = vec3_new(0.0, 0.0, 0.0);
	b = vec3_new(1.0, -1.0, 2.0);
	result = vec3_sub(a, b);
	expected = vec3_new(-1.0, 1.0, -2.0);
	assert(vec3_equals(result, expected));
	
	printf("✓ vec3_sub tests passed\n");
}/*
** 
Test vec3_mult function
*/
void	test_vec3_mult(void)
{
	t_vec3	v, result, expected;

	printf("Testing vec3_mult...\n");
	
	v = vec3_new(1.0, 2.0, 3.0);
	result = vec3_mult(v, 2.0);
	expected = vec3_new(2.0, 4.0, 6.0);
	assert(vec3_equals(result, expected));
	
	v = vec3_new(-1.0, 0.5, -2.0);
	result = vec3_mult(v, -3.0);
	expected = vec3_new(3.0, -1.5, 6.0);
	assert(vec3_equals(result, expected));
	
	v = vec3_new(1.0, 2.0, 3.0);
	result = vec3_mult(v, 0.0);
	expected = vec3_new(0.0, 0.0, 0.0);
	assert(vec3_equals(result, expected));
	
	printf("✓ vec3_mult tests passed\n");
}

/*
** Test vec3_div function
*/
void	test_vec3_div(void)
{
	t_vec3	v, result, expected;

	printf("Testing vec3_div...\n");
	
	v = vec3_new(2.0, 4.0, 6.0);
	result = vec3_div(v, 2.0);
	expected = vec3_new(1.0, 2.0, 3.0);
	assert(vec3_equals(result, expected));
	
	v = vec3_new(-3.0, 1.5, -6.0);
	result = vec3_div(v, -3.0);
	expected = vec3_new(1.0, -0.5, 2.0);
	assert(vec3_equals(result, expected));
	
	// Test division by zero (should return zero vector)
	v = vec3_new(1.0, 2.0, 3.0);
	result = vec3_div(v, 0.0);
	expected = vec3_new(0.0, 0.0, 0.0);
	assert(vec3_equals(result, expected));
	
	printf("✓ vec3_div tests passed\n");
}

/*
** Test vec3_dot function
*/
void	test_vec3_dot(void)
{
	t_vec3	a, b;
	double	result;

	printf("Testing vec3_dot...\n");
	
	a = vec3_new(1.0, 2.0, 3.0);
	b = vec3_new(4.0, 5.0, 6.0);
	result = vec3_dot(a, b);
	assert(double_equals(result, 32.0)); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
	
	a = vec3_new(1.0, 0.0, 0.0);
	b = vec3_new(0.0, 1.0, 0.0);
	result = vec3_dot(a, b);
	assert(double_equals(result, 0.0)); // Perpendicular vectors
	
	a = vec3_new(1.0, 0.0, 0.0);
	b = vec3_new(1.0, 0.0, 0.0);
	result = vec3_dot(a, b);
	assert(double_equals(result, 1.0)); // Same unit vectors
	
	printf("✓ vec3_dot tests passed\n");
}

/*
** Test vec3_cross function
*/
void	test_vec3_cross(void)
{
	t_vec3	a, b, result, expected;

	printf("Testing vec3_cross...\n");
	
	// Standard basis vectors
	a = vec3_new(1.0, 0.0, 0.0);
	b = vec3_new(0.0, 1.0, 0.0);
	result = vec3_cross(a, b);
	expected = vec3_new(0.0, 0.0, 1.0);
	assert(vec3_equals(result, expected));
	
	a = vec3_new(0.0, 1.0, 0.0);
	b = vec3_new(0.0, 0.0, 1.0);
	result = vec3_cross(a, b);
	expected = vec3_new(1.0, 0.0, 0.0);
	assert(vec3_equals(result, expected));
	
	// Parallel vectors should give zero vector
	a = vec3_new(1.0, 2.0, 3.0);
	b = vec3_new(2.0, 4.0, 6.0);
	result = vec3_cross(a, b);
	expected = vec3_new(0.0, 0.0, 0.0);
	assert(vec3_equals(result, expected));
	
	printf("✓ vec3_cross tests passed\n");
}/*

** Test vec3_length_squared function
*/
void	test_vec3_length_squared(void)
{
	t_vec3	v;
	double	result;

	printf("Testing vec3_length_squared...\n");
	
	v = vec3_new(3.0, 4.0, 0.0);
	result = vec3_length_squared(v);
	assert(double_equals(result, 25.0)); // 3² + 4² + 0² = 9 + 16 + 0 = 25
	
	v = vec3_new(1.0, 1.0, 1.0);
	result = vec3_length_squared(v);
	assert(double_equals(result, 3.0)); // 1² + 1² + 1² = 3
	
	v = vec3_new(0.0, 0.0, 0.0);
	result = vec3_length_squared(v);
	assert(double_equals(result, 0.0));
	
	printf("✓ vec3_length_squared tests passed\n");
}

/*
** Test vec3_length function
*/
void	test_vec3_length(void)
{
	t_vec3	v;
	double	result;

	printf("Testing vec3_length...\n");
	
	v = vec3_new(3.0, 4.0, 0.0);
	result = vec3_length(v);
	assert(double_equals(result, 5.0)); // sqrt(3² + 4²) = sqrt(25) = 5
	
	v = vec3_new(1.0, 1.0, 1.0);
	result = vec3_length(v);
	assert(double_equals(result, sqrt(3.0))); // sqrt(3)
	
	v = vec3_new(0.0, 0.0, 0.0);
	result = vec3_length(v);
	assert(double_equals(result, 0.0));
	
	printf("✓ vec3_length tests passed\n");
}

/*
** Test vec3_normalize function
*/
void	test_vec3_normalize(void)
{
	t_vec3	v, result, expected;
	double	length;

	printf("Testing vec3_normalize...\n");
	
	v = vec3_new(3.0, 4.0, 0.0);
	result = vec3_normalize(v);
	expected = vec3_new(0.6, 0.8, 0.0); // (3/5, 4/5, 0)
	assert(vec3_equals(result, expected));
	
	// Check that normalized vector has length 1
	length = vec3_length(result);
	assert(double_equals(length, 1.0));
	
	v = vec3_new(1.0, 0.0, 0.0);
	result = vec3_normalize(v);
	expected = vec3_new(1.0, 0.0, 0.0);
	assert(vec3_equals(result, expected));
	
	// Test zero vector normalization (should return zero vector)
	v = vec3_new(0.0, 0.0, 0.0);
	result = vec3_normalize(v);
	expected = vec3_new(0.0, 0.0, 0.0);
	assert(vec3_equals(result, expected));
	
	printf("✓ vec3_normalize tests passed\n");
}

/*
** Main test function
*/
int	main(void)
{
	printf("=== Vector Operations Unit Tests ===\n\n");
	
	test_vec3_new();
	test_vec3_add();
	test_vec3_sub();
	test_vec3_mult();
	test_vec3_div();
	test_vec3_dot();
	test_vec3_cross();
	test_vec3_length_squared();
	test_vec3_length();
	test_vec3_normalize();
	
	printf("\n=== All vector tests passed! ===\n");
	return (0);
}