/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/19 09:06:21 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static inline void	free_camera_tokens(char *pos, char *dir, char *fov)
{
	safe_free((void **)&pos);
	safe_free((void **)&dir);
	safe_free((void **)&fov);
}

/*
** parse_camera - Parse camera element from line (C x,y,z nx,ny,nz fov)
** line: Line to parse
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
int	parse_camera(char *line, t_scene *scene)
{
	char	*fov_str;
	char	*position_str;
	char	*direction_str;

	if (!line || !scene)
		return (0);
	position_str = get_next_token(&line);
	direction_str = get_next_token(&line);
	fov_str = get_next_token(&line);
	if (!position_str || !direction_str || !fov_str)
		return (0);
	if (!parse_vec3(position_str, &scene->camera.position) || !parse_vec3(\
				direction_str, &scene->camera.direction) || !parse_double(\
					fov_str, &scene->camera.fov))
		return (free_camera_tokens(position_str, direction_str, fov_str), 0);
	if (scene->camera.fov <= 0 || scene->camera.fov >= 180)
		return (free_camera_tokens(position_str, direction_str, fov_str), 0);
	scene->camera.up = vec3_new(0, 1, 0);
	scene->camera.right = vec3_normalize(vec3_cross(scene->camera.direction, \
				scene->camera.up));
	scene->camera.up = vec3_normalize(vec3_cross(scene->camera.right, \
				scene->camera.direction));
	scene->camera.move_speed = 0.5;
	scene->camera.rotate_speed = 0.05;
	return (free_camera_tokens(position_str, direction_str, fov_str), 1);
}

static inline void	free_ambient_tokens(char *ratio, char *color)
{
	safe_free((void **)&ratio);
	safe_free((void **)&color);
}

/*
** parse_ambient - Parse ambient lighting from line (A ratio r,g,b)
** line: Line to parse
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
int	parse_ambient(char *line, t_scene *scene)
{
	t_color	color;
	double	ratio;
	char	*ratio_str;
	char	*color_str;

	if (!line || !scene)
		return (0);
	ratio_str = get_next_token(&line);
	color_str = get_next_token(&line);
	if (!ratio_str || !color_str)
		return (0);
	if (!parse_double(ratio_str, &ratio) || !parse_color(color_str, &color))
		return (free_ambient_tokens(ratio_str, color_str), 0);
	if (ratio < 0.0 || ratio > 1.0)
		return (free_ambient_tokens(ratio_str, color_str), 0);
	scene->ambient_ratio = ratio;
	scene->ambient = color;
	return (free_ambient_tokens(ratio_str, color_str), 1);
}
