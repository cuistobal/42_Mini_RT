/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:44:17 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/16 09:05:36 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static inline void	swap(double *t0, double *t1)
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
    int (i) = 0;
    double	(tmin), (tmax), (t0), (t1), (invD), (orig), (dir), (bmin), bmax;
    tmin = -INFINITY;
    tmax = INFINITY;
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
