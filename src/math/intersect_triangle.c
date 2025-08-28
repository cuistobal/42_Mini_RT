/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_triangle.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/28 15:54:05 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static inline int	check_barycentric_coords(double u, double v)
{
    return !(u < 0.0 || u > 1.0 || v < 0.0 || (u + v) > 1.0);
}

static inline void	set_edge_vectors(t_vec3 *edge1, t_vec3 *edge2, t_vec3 v0, t_vec3 v1, t_vec3 v2)
{
	*edge1 = vec3_sub(v1, v0);
	*edge2 = vec3_sub(v2, v0);
}

static inline double	compute_h_return_a(t_vec3 dir, t_vec3 edge1, t_vec3 edge2, t_vec3 *h)
{
	*h = vec3_cross(dir, edge2);
	return (vec3_dot(edge1, *h));
}

typedef s_inter
{
	t_vec3	edge;
	t_vec3	edge2
	t_vec3	h;
	t_vec3	s;
	t_vec3	q;
	double	a;
	double	f;
	double	u;
	double	v;
	double	t;

}	t_inter;

static inline void	set_norm(t_inter *n, t_object *triangle, t_ray dir)
{
	n->edge1 = vec_sub();
	n->edge2 = vec_sub();
	n->h = ;
	n->s = ;
	n->q = ;
	n->a = ;
	n->f = ;
	n->u = ;
	n->v = ;
	n->t = ;
}

// Using the moller trumbore algorithm to define if the tested ray/point is a barycentric point
double	intersect_triangle(t_ray ray, t_object *triangle)
{
/*
	t_vec3	edge1, edge2, h, s, q;
	double	a, f, u, v, t;
*/
	t_inter	n;

	set_norm(&n);

	set_edge_vectors(&edge1, &edge2, triangle->position, triangle->normal, triangle->axis);
	a = compute_h_return_a(ray.direction, edge1, edge2, &h);
	if (fabs(a) < EPSILON)
		return (-1.0);
	f = 1.0 / a;
	s = vec3_sub(ray.origin, triangle->position);
	u = f * vec3_dot(s, h);
	if (!check_barycentric_coords(u, 0.0))
		return (-1.0);
	q = vec3_cross(s, edge1);
	v = f * vec3_dot(ray.direction, q);
	if (!check_barycentric_coords(u, v))
		return (-1.0);
	t = f * vec3_dot(edge2, q);
	return ((t > EPSILON) * t + (t <= EPSILON) * -1.0);
}
