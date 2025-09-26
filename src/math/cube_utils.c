/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/09/26 07:51:22 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	get_cube_bounds(t_object *cube, t_vec3 *cube_min, t_vec3 *cube_max)
{
	t_vec3	radius_vec;

	radius_vec = vec3_new(cube->radius, cube->radius, cube->radius);
	*cube_min = vec3_sub(cube->position, radius_vec);
	*cube_max = vec3_add(cube->position, radius_vec);
}

void	swap_values(double *t1, double *t2)
{
	double	temp;

	temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}

int	test_slab_intersection(double ray_dir, double ray_orig,
	double slab_min, double slab_max)
{
	if (fabs(ray_dir) < EPSILON)
	{
		if (ray_orig < slab_min || ray_orig > slab_max)
			return (0);
	}
	return (1);
}

t_sinter	calculate_slab_intersections(double ray_dir,
	double ray_orig, double slab_min, double slab_max)
{
	t_sinter	result;

	result.t1 = (slab_min - ray_orig) / ray_dir;
	result.t2 = (slab_max - ray_orig) / ray_dir;
	if (result.t1 > result.t2)
		swap_values(&result.t1, &result.t2);
	return (result);
}

t_sresult	update_intersection_interval(t_sresult prev,
	double t1, double t2)
{
	t_sresult	result;

	result = prev;
	if (t1 > result.t_min)
		result.t_min = t1;
	if (t2 < result.t_max)
		result.t_max = t2;
	if (result.t_min > result.t_max)
		result.valid = 0;
	return (result);
}
