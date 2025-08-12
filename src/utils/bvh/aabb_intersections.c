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

static void	swap(double *t0, double *t1, double invD)
{
	double	tmp;

	if (invD < 0.0)
	{
		tmp = *t0;
		*t0 = *t1;
		*t1 = tmp;
	}
}

/*
** intersect_aabb - Fast ray-AABB intersection test
** Returns 1 if ray intersects the bounding box, 0 otherwise
** We need to split that shit for norminette
*/
int	intersect_aabb(double ray_orig[3], double ray_dir[3], double box_min[3], double box_max[3])
{
	int i;
    double (tmin), (tmax), (invD), (t0), t1;

	tmin = -INFINITY;
    tmax = INFINITY;
    for (i = 0; i < 3; i++)
    {
        if (fabs(ray_dir[i]) < EPSILON)
        {
            if (ray_orig[i] < box_min[i] || ray_orig[i] > box_max[i])
                return (0);
        }
        else
        {
            invD = 1.0 / ray_dir[i];
            t0 = (box_min[i] - ray_orig[i]) * invD;
            t1 = (box_max[i] - ray_orig[i]) * invD;
			swap(&t0, &t1, invD);
            if (t0 > tmin)
                tmin = t0;
            if (t1 < tmax)
                tmax = t1;
            if (tmin > tmax)
                return (0);
        }
    }
    return (tmax > EPSILON);
}
