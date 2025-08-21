/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:44:17 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 10:20:42 by chrleroy         ###   ########.fr       */
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

static inline int	helper(double *tmin, double *tmax, double t0, double t1)
{
	*tmin = fmax(*tmin, t0);
	*tmax = fmin(*tmax, t1);
	return ((int)(*tmin > *tmax));
}

/*
** intersect_aabb - Fast ray-AABB intersection test
** Returns 1 if ray intersects the bounding box, 0 otherwise
** We need to split that shit for norminette
*/
int	intersect_aabb_query(t_aabb_query *q)
{
	int (i) = 0;
	double (tmn), (tmx), (t0), (t1), (invD), (org), (dir), (bmn), (bmx), (adr);
	tmx = INFINITY;
	tmn = -INFINITY;
	while (i < 3)
	{
		dir = ((double *)&q->dir)[i];
		org = ((double *)&q->origin)[i];
		bmn = ((double *)&q->box.min)[i];
		bmx = ((double *)&q->box.max)[i++];
		adr = fabs(dir);
		
		// Si le rayon est parallèle à l'axe (presque zéro dans cette dimension)
		if (adr < EPSILON)
		{
			// Si le point d'origine est en dehors de la boîte, pas d'intersection
			if (org < bmn - EPSILON || org > bmx + EPSILON)
				return (0);
			// Sinon continuer avec les autres axes
		}
		else
		{
			invD = 1.0 / dir;
			t0 = (bmn - org) * invD;
			t1 = (bmx - org) * invD;
			if (invD < 0.0)
				swap(&t0, &t1);
				
			// Mettre à jour tmn et tmx
			tmn = fmax(tmn, t0);
			tmx = fmin(tmx, t1);
			
			// Vérifier si le rayon manque la boîte
			if (tmn > tmx || tmx < 0)
				return (0);
		}
	}
	
	// Mettre à jour les valeurs tmin et tmax
	q->tmin = tmn;
	q->tmax = tmx;
	
	// Retourner 1 si l'intersection est valide (devant le rayon)
	return (tmx > 0);
}
