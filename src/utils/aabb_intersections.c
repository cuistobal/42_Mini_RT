/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:44:17 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/26 07:44:27 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static inline void	update_intersection_vars(t_aabb_query_vars *vars)
{
	vars->invd = 1.0 / vars->dir;
	vars->t0 = (vars->bmn - vars->org) * vars->invd;
	vars->t1 = (vars->bmx - vars->org) * vars->invd;
	if (vars->invd < 0.0)
		swap(&vars->t0, &vars->t1);
	vars->tmn = fmax(vars->tmn, vars->t0);
	vars->tmx = fmin(vars->tmx, vars->t1);
}

static inline int	helper(double *tmin, double *tmax, double t0, double t1)
{
	*tmin = fmax(*tmin, t0);
	*tmax = fmin(*tmax, t1);
	return ((int)(*tmin > *tmax));
}

static inline void	init_vars(t_aabb_query_vars *vars)
{
	vars->i = 0;
	vars->tmx = INFINITY;
	vars->tmn = -INFINITY;
}

static inline void	update_vars(t_aabb_query_vars *vars, t_aabb_query *q)
{
	vars->dir = ((double *)&q->dir)[vars->i];
	vars->org = ((double *)&q->origin)[vars->i];
	vars->bmn = ((double *)&q->box.min)[vars->i];
	vars->bmx = ((double *)&q->box.max)[vars->i++];
	vars->adr = fabs(vars->dir);
}

/*
** intersect_aabb - Fast ray-AABB intersection test
** Returns 1 if ray intersects the bounding box, 0 otherwise
** We need to split that shit for norminette
*/
int	intersect_aabb_query(t_aabb_query *q)
{
	t_aabb_query_vars	vars;

	init_vars(&vars);
	while (vars.i < 3)
	{
		update_vars(&vars, q);
		if (vars.adr < EPSILON)
		{
			if (vars.org < vars.bmn - EPSILON || vars.org > vars.bmx + EPSILON)
				return (0);
		}
		else
		{
			update_intersection_vars(&vars);
			if (vars.tmn > vars.tmx || vars.tmx < 0)
				return (0);
		}
	}
	q->tmin = vars.tmn;
	q->tmax = vars.tmx;
	return (vars.tmx > 0);
}
