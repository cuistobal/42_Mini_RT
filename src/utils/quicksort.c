/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:47:40 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 09:24:39 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	get_axis_value(t_object *obj, int axis)
{
	return (obj->position.x * (!!(axis & 1)) + obj->position.y * \
		(!!(axis & 2)) + obj->position.z * (!!(axis & 4)));
}

static void	swap_objects(t_object **a, t_object **b)
{
	t_object	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	partition(t_object **arr, int low, int high, int axis)
{
	int		i;
	int		j;
	double	pivot;

	j = low;
	i = low - 1;
	pivot = get_axis_value(arr[high], axis);
	while (j < high)
	{
		if (get_axis_value(arr[j], axis) < pivot)
		{
			i++;
			swap_objects(&arr[i], &arr[j]);
		}
		j++;
	}
	swap_objects(&arr[i + 1], &arr[high]);
	return (i + 1);
}

static void	quicksort_objects(t_object **arr, int low, int high, int axis)
{
	int	pivot;

	if (low < high)
	{
		pivot = partition(arr, low, high, axis);
		quicksort_objects(arr, low, pivot - 1, axis);
		quicksort_objects(arr, pivot + 1, high, axis);
	}
}

/*
** sort_objects_axis - Naive bubble sort for BVH splitting (axis: 0=x, 1=y, 2=z)
*/
void	sort_objects_axis(t_object **objects, int count, int axis)
{
	int	mask;

	if (count <= 1)
		return ;
	mask = (1 << axis);
	quicksort_objects(objects, 0, count - 1, mask);
}
