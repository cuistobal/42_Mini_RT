/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/09 10:20:14 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** vec3_new - Create a new vector with given coordinates
*/
t_vec3	vec3_new(double x, double y, double z)
{
	return ((t_vec3){
		.x = x,
		.y = y,
		.z = z
	});
}

/*
** vec3_add - Add two vectors component-wise
*/
t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	});
}

/*
** vec3_sub - Subtract vector b from vector a component-wise
*/
t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	});
}

/*
** vec3_mult - Multiply vector by scalar
*/
t_vec3	vec3_mult(t_vec3 v, double scalar)
{
	return ((t_vec3){
		.x = v.x * scalar,
		.y = v.y * scalar,
		.z = v.z * scalar
	});
}

/*
** vec3_div - Divide vector by scalar
*/
t_vec3	vec3_div(t_vec3 v, double scalar)
{
	if (fabs(scalar) < EPSILON)
		return (vec3_new(0, 0, 0));
	return ((t_vec3){
		.x = v.x / scalar,
		.y = v.y / scalar,
		.z = v.z / scalar
	});
}