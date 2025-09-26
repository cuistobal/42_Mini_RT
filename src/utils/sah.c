/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:24:05 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/26 08:18:28 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
**
*/
static void	fill_bounds(t_object **objects, int c,
	t_aabb *left, t_aabb *right)
{
	int	i;

	i = 1;
	left[0] = get_object_bounds(objects[0]);
	while (i < c)
	{
		left[i] = aabb_union(left[i - 1], get_object_bounds(objects[i]));
		i++;
	}
	right[c - 1] = get_object_bounds(objects[c - 1]);
	i = c - 2;
	while (i >= 0)
	{
		right[i] = aabb_union(right[i + 1], get_object_bounds(objects[i]));
		i--;
	}
}

/*
static double	sah_cost(double left_area, double right_area,
	int left_c, int right_c)
{
	return (left_area * left_c + right_area * right_c);
}
*/

/*
**
*/
static void	allocate_bounds(t_aabb **left_bounds, t_aabb **right_bounds, \
		int c)
{
	if (c <= 0)
		return ;
	*left_bounds = safe_malloc(sizeof(t_aabb) * c);
	*right_bounds = safe_malloc(sizeof(t_aabb) * c);
}

/*
**
*/
static void	variable_setup(int *axis, int *best_axis, double *best_cost, \
		int *best_index)
{
	*axis = 0;
	*best_axis = 0;
	*best_index = 0;
	*best_cost = INFINITY;
}

static inline int	fuck_the_norm(t_aabb **lbounds, t_aabb **rbounds, int ret)
{
	safe_free((void **)lbounds);
	safe_free((void **)rbounds);
	return (ret);
}

/*
** La norminette n'a qu'a bien se tenir
** Probably the least maintainable piece of crap within this project, thanks to
** norminette. We could have created a specific struct in this regard.
*/
int	find_sah_split(t_object **objects, int c, int *best_axis)
{
	t_sah_split_vars	v;

	variable_setup(&v.axis, best_axis, &v.best_cost, &v.best_split);
	allocate_bounds(&v.left_bounds, &v.right_bounds, c);
	while (v.axis < 3)
	{
		v.split = 1;
		sort_objects_axis(objects, c, v.axis);
		fill_bounds(objects, c, v.left_bounds, v.right_bounds);
		while (v.split < c)
		{
			v.left_area = aabb_surface(v.left_bounds[v.split - 1]);
			v.right_area = aabb_surface(v.right_bounds[v.split]);
			v.cost = v.left_area * v.split + v.right_area * (c - v.split);
			if (v.cost < v.best_cost)
			{
				v.best_cost = v.cost;
				*best_axis = v.axis;
				v.best_split = v.split;
			}
			v.split++;
		}
		v.axis++;
	}
	return (fuck_the_norm(&v.left_bounds, &v.right_bounds, v.best_split));
}
