/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:44:17 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/12 08:49:27 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minirt.h"

// static int helper(double *t1, double *t2, double *t_min, double *t_max)
// {
// 	double temp;

// 	if (*t1 > *t2)
// 	{
// 		temp = *t1;
// 		*t1 = *t2;
// 		*t2 = temp;
// 	}
// 	if (*t1 > *t_min)
// 		*t_min = *t1;
// 	if (*t2 < *t_max)
// 		*t_max = *t2;
// 	if (*t_min > *t_max)
// 		return (0);
// }
// /*
// ** intersect_aabb - Fast ray-AABB intersection test
// ** Returns 1 if ray intersects the bounding box, 0 otherwise
// ** We need to split that shit for norminette
// */
// int	intersect_aabb(double ray_orig[], double ray_dir[], double box_min[], \
// 		double box_max[])
// {
// 	int		i;
// 	double	t1;
// 	double	t2;
// 	double	t_min;
// 	double	t_max;

// 	i = 0;
// 	t_min = -INFINITY;
// 	t_max = INFINITY;
// 	while (i < 3)
// 	{
// 		if (fabs(ray_dir[i]) < EPSILON)
// 		{
// 			if (ray_orig[i] < box_min[i] || ray_orig[i] > box_max[i])
// 				return (0);
// 		}
// 		else
// 		{
// 			t1 = (box_min[i] - ray_orig[i]) / ray_dir[i];
// 			t2 = (box_max[i] - ray_orig[i]) / ray_dir[i];
// 			if (helper(&t1, &t2, &t_min, &t_max))
// 				return (0);
// 		}
// 		i++;
// 	}
// 	return (t_max > EPSILON);
// }

static void	swap(double *t0, double *t1)
{
    double	tmp;

    tmp = *t0;
    *t0 = *t1;
    *t1 = tmp;
}

/*
** intersect_aabb - Fast ray-AABB intersection test
** Returns 1 if ray intersects the bounding box, 0 otherwise
** We need to split that shit for norminette
*/

int	intersect_aabb_query(t_aabb_query *q)
{
	int i;

	double	(tmin), (tmax), (t0), (t1), (invD), (orig), (dir), (bmin), bmax;
    tmin = -INFINITY;
    tmax = INFINITY;
    i = 0;
    while (i < 3)
    {
        orig = ((double *)&q->origin)[i];
        dir = ((double *)&q->dir)[i];
        bmin = ((double *)&q->box.min)[i];
        bmax = ((double *)&q->box.max)[i];
        if (fabs(dir) < EPSILON)
        {
            if (orig < bmin || orig > bmax)
                return (0);
        }
        else
        {
            invD = 1.0 / dir;
            t0 = (bmin - orig) * invD;
            t1 = (bmax - orig) * invD;
            if (invD < 0.0)
                swap(&t0, &t1);
            if (t0 > tmin)
                tmin = t0;
            if (t1 < tmax)
                tmax = t1;
            if (tmin > tmax)
                return (0);
        }
        i++;
    }
    q->tmin = tmin;
    q->tmax = tmax;
    return (tmax > EPSILON);
}
