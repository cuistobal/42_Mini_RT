/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** vec3_new - Create a new vector with given coordinates
*/
t_vec3	vec3_new(double x, double y, double z)
{
	t_vec3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

/*
** vec3_add - Add two vectors component-wise
*/
t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

/*
** vec3_sub - Subtract vector b from vector a component-wise
*/
t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

/*
** vec3_mult - Multiply vector by scalar
*/
t_vec3	vec3_mult(t_vec3 v, double scalar)
{
	t_vec3	result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	return (result);
}

/*
** vec3_div - Divide vector by scalar
*/
t_vec3	vec3_div(t_vec3 v, double scalar)
{
	t_vec3	result;

	if (fabs(scalar) < EPSILON)
		return (vec3_new(0, 0, 0));
	result.x = v.x / scalar;
	result.y = v.y / scalar;
	result.z = v.z / scalar;
	return (result);
}

/*
** vec3_dot - Calculate dot product of two vectors
*/
double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

/*
** vec3_cross - Calculate cross product of two vectors
*/
t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

/*
** vec3_length_squared - Calculate squared length of vector (for optimization)
*/
double	vec3_length_squared(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
** vec3_length - Calculate length (magnitude) of vector
*/
double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}

/*
** vec3_normalize - Normalize vector to unit length
*/
t_vec3	vec3_normalize(t_vec3 v)
{
	double	length;

	length = vec3_length(v);
	if (length < EPSILON)
		return (vec3_new(0, 0, 0));
	return (vec3_div(v, length));
}