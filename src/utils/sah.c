/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:24:05 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 09:17:56 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
**
*/
static void	fill_bounds(t_object **objects, int count,
	t_aabb *left, t_aabb *right)
{
	int	i;

	i = 1;
	left[0] = get_object_bounds(objects[0]);
	while (i < count)
	{
		left[i] = aabb_union(left[i - 1], get_object_bounds(objects[i]));
		i++;
	}
	right[count - 1] = get_object_bounds(objects[count - 1]);
	i = count - 2;
	while (i >= 0)
	{
		right[i] = aabb_union(right[i + 1], get_object_bounds(objects[i]));
		i--;
	}
}

static double	sah_cost(double left_area, double right_area,
	int left_count, int right_count)
{
	return (left_area * left_count + right_area * right_count);
}

/*
**
*/
static void	allocate_bounds(t_aabb **left_bounds, t_aabb **right_bounds, \
		int count)
{
	*left_bounds = safe_malloc(sizeof(t_aabb) * count);
	*right_bounds = safe_malloc(sizeof(t_aabb) * count);
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

/*
** La norminette n'a qu'a bien se tenir
** Probably the least maintainable piece of crap within this project, thanks to
** norminette. We could have created a specific struct in this regard.
*/
int	find_sah_split(t_object **objects, int count, int *best_axis)
{
	t_sah_split_vars vars;

	variable_setup(&vars.axis, best_axis, &vars.best_cost, &vars.best_split);
	allocate_bounds(&vars.left_bounds, &vars.right_bounds, count);
	while (vars.axis++ < 3)
	{
		vars.split = 1;
		sort_objects_axis(objects, count, vars.axis);
		fill_bounds(objects, count, vars.left_bounds, vars.right_bounds);
		while (vars.split++ < count)
		{
			vars.left_area = aabb_surface(vars.left_bounds[vars.split - 1]);
			vars.right_area = aabb_surface(vars.right_bounds[vars.split]);
			vars.cost = sah_cost(vars.left_area, vars.right_area, vars.split, count - vars.split);
			if (vars.cost < vars.best_cost)
			{
				vars.best_cost = vars.cost;
				*best_axis = vars.axis;
				vars.best_split = vars.split;
			}
		}
	}
	return (safe_free((void **)&vars.left_bounds), \
			safe_free((void **)&vars.right_bounds), vars.best_split);
}
