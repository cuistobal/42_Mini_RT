/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:24:05 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/19 08:45:47 by chrleroy         ###   ########.fr       */
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
	while (i < count )
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
int	find_sah_split(t_object **objects, int count, int *best_axis, \
		int *best_split)
{
	int (axis), (split);
	t_aabb (*left_bounds);
	t_aabb (*right_bounds);
	double (left_area), (right_area), (cost), best_cost;
	variable_setup(&axis, best_axis, &best_cost, best_split);
	allocate_bounds(&left_bounds, &right_bounds, count);


	// Calculer les centroids pour chaque object etudie -> inclus dans la structure

	// Choisir l'axe le plus long

	// Repartir les objects en fonction de la position de leur centroid au 
	// centroid de la box


	while (axis++ < 3)
	{
		split = 1;
		// sort_objects_axis(objects, count, axis);
		fill_bounds(objects, count, left_bounds, right_bounds);
		while (split++ < count)
		{
			left_area = aabb_surface(left_bounds[split - 1]);
			right_area = aabb_surface(right_bounds[split]);
			// printf("%f vs %f\n", left_area, right_area);
			cost = sah_cost(left_area, right_area, split, count - split);
			if (cost < best_cost)
			{
				best_cost = cost;
				*best_axis = axis;
				*best_split = split;
			}
		}
	}
	return (safe_free((void **)&left_bounds), \
			safe_free((void **)&right_bounds), *best_split);
}