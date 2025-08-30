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
    return (!(u < 0.0 || u > 1.0 || v < 0.0 || (u + v) > 1.0));
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

static inline void	set_norm(t_inter *n, t_object *triangle, t_ray ray)
{
    // Utilise les membres de t_object pour les sommets du triangle
    // position: sommet 0, normal: sommet 1, axis: sommet 2
    t_vec3 v0 = triangle->position;
    t_vec3 v1 = triangle->normal;
    t_vec3 v2 = triangle->axis;

    n->edge1 = vec3_sub(v1, v0);
    n->edge2 = vec3_sub(v2, v0);
    n->h = vec3_cross(ray.direction, n->edge2);
    n->a = vec3_dot(n->edge1, n->h);
    n->f = 1.0 / n->a;
    n->s = vec3_sub(ray.origin, v0);
    n->u = n->f * vec3_dot(n->s, n->h);
    n->q = vec3_cross(n->s, n->edge1);
    n->v = n->f * vec3_dot(ray.direction, n->q);
    n->t = n->f * vec3_dot(n->edge2, n->q);
}

double	intersect_triangle(t_ray ray, t_object *triangle)
{
    t_inter n;

    set_norm(&n, triangle, ray);

    if (fabs(n.a) < EPSILON)
        return (-1.0);
    if (!check_barycentric_coords(n.u, 0.0))
        return (-1.0);
    if (!check_barycentric_coords(n.u, n.v))
        return (-1.0);
    return ((n.t > EPSILON) ? n.t : -1.0);
}
