/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	get_cone_projections(t_ray ray, t_object *cone, t_vec3 *proj_ray,
	t_vec3 *proj_oc)
{
	t_vec3	oc;
	double	axis_dot_ray;
	double	axis_dot_oc;

	oc = vec3_sub(ray.origin, cone->position);
	axis_dot_ray = vec3_dot(ray.direction, cone->axis);
	axis_dot_oc = vec3_dot(oc, cone->axis);
	*proj_ray = vec3_sub(ray.direction, vec3_mult(cone->axis, axis_dot_ray));
	*proj_oc = vec3_sub(oc, vec3_mult(cone->axis, axis_dot_oc));
}
