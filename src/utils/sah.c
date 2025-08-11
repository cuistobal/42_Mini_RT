/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:24:05 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/10 14:35:32 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	aabb_surface(t_aabb box)
{
    double dx;
    double dy;
    double dz;

	dx = box.max.x - box.min.x;
    dy = box.max.y - box.min.y;
    dz = box.max.z - box.min.z;
    return 2.0 * (dx * dy + dx * dz + dy * dz);
}

// Utilitaire pour comparer les objets selon un axe
static int	compare_obj_x(const void *a, const void *b)
{
    const t_object *oa;
    const t_object *ob;

	oa = *(const t_object **)a;
	ob = *(const t_object **)b;
    return (oa->position.x > ob->position.x) - \
		(oa->position.x < ob->position.x);
}

// Calcule le meilleur split selon SAH
static int	find_sah_split(t_object **objects, int count, int *best_axis, int *best_index)
{
    int axis, i, split, best_split = 1;
    double best_cost = INFINITY;
    int (*comparators[3])(const void *, const void *) = {compare_obj_x, compare_obj_y, compare_obj_z};

    for (axis = 0; axis < 3; axis++)
    {
        qsort(objects, count, sizeof(t_object *), comparators[axis]);
        for (split = 1; split < count; split++)
        {
            t_aabb left_box = get_object_bounds(objects[0]);
            t_aabb right_box = get_object_bounds(objects[split]);
            for (i = 1; i < split; i++)
                left_box = aabb_union(left_box, get_object_bounds(objects[i]));
            for (i = split + 1; i < count; i++)
                right_box = aabb_union(right_box, get_object_bounds(objects[i]));
            double left_area = aabb_surface(left_box);
            double right_area = aabb_surface(right_box);
            double cost = left_area * split + right_area * (count - split);
            if (cost < best_cost)
            {
                best_cost = cost;
                *best_axis = axis;
                *best_index = split;
                best_split = split;
            }
        }
    }
    return best_split;
}

//


static int	compare_obj_y(const void *a, const void *b)
{
    const t_object *oa = *(const t_object **)a;
    const t_object *ob = *(const t_object **)b;
    return (oa->position.y > ob->position.y) - (oa->position.y < ob->position.y);
}

static int	compare_obj_z(const void *a, const void *b)
{
    const t_object *oa = *(const t_object **)a;
    const t_object *ob = *(const t_object **)b;
    return (oa->position.z > ob->position.z) - (oa->position.z < ob->position.z);
}

static int	find_sah_split(t_object **objects, int count, int *best_axis, int *best_index)
{
    int		axis;
    int		split;
    int		i;
    double	best_cost;
    double	cost;
    t_aabb	*left_bounds;
    t_aabb	*right_bounds;

    best_cost = INFINITY;
    *best_axis = 0;
    *best_index = 1;
    left_bounds = malloc(sizeof(t_aabb) * count);
    right_bounds = malloc(sizeof(t_aabb) * count);
    axis = 0;
    while (axis < 3)
    {
        sort_objects_axis(objects, count, axis);
        left_bounds[0] = get_object_bounds(objects[0]);
        i = 1;
        while (i < count)
        {
            left_bounds[i] = aabb_union(left_bounds[i - 1],
                    get_object_bounds(objects[i]));
            i++;
        }
        right_bounds[count - 1] = get_object_bounds(objects[count - 1]);
        i = count - 2;
        while (i >= 0)
        {
            right_bounds[i] = aabb_union(right_bounds[i + 1],
                    get_object_bounds(objects[i]));
            i--;
        }
        split = 1;
        while (split < count)
        {
            double	left_area = aabb_surface(left_bounds[split - 1]);
            double	right_area = aabb_surface(right_bounds[split]);
            cost = left_area * split + right_area * (count - split);
            if (cost < best_cost)
            {
                best_cost = cost;
                *best_axis = axis;
                *best_index = split;
            }
            split++;
        }
        axis++;
    }
    free(left_bounds);
    free(right_bounds);
    return (*best_index);
}
