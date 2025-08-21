/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** add_light_to_scene - Add a light to the scene's linked list
** scene: Scene structure
** light: Light to add
*/
static void	add_light_to_scene(t_scene *scene, t_light *light)
{
	t_light	*current;

	if (!scene || !light)
		return ;
	if (!scene->lights)
	{
		scene->lights = light;
		return ;
	}
	current = scene->lights;
	while (current->next)
		current = current->next;
	current->next = light;
}

static inline void	free_light_tokens(char *pos, char *bright, char *col)
{
	safe_free((void **)&pos);
	safe_free((void **)&bright);
	safe_free((void **)&col);
}

/*
** parse_light - Parse light element from line (L x,y,z brightness r,g,b)
** line: Line to parse
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
int	parse_light(char *line, t_scene *scene)
{
	char	*position_str;
	char	*brightness_str;
	char	*color_str;
	t_vec3	position;
	double	brightness;
	t_color	color;
	t_light	*light;

	if (!line || !scene)
		return (0);
	position_str = get_next_token(&line);
	brightness_str = get_next_token(&line);
	color_str = get_next_token(&line);
	if (!position_str || !brightness_str || !color_str)
		return (0);
	if (!parse_vec3(position_str, &position) ||
		!parse_double(brightness_str, &brightness) ||
		!parse_color(color_str, &color))
		return (free_light_tokens(position_str, brightness_str, color_str), 0);
	if (brightness < 0.0 || brightness > 1.0)
		return (free_light_tokens(position_str, brightness_str, color_str), 0);
	light = safe_malloc(sizeof(t_light));
	light->position = position;
	light->intensity = brightness;
	light->color = color;
	light->next = NULL;
	add_light_to_scene(scene, light);
	return (free_light_tokens(position_str, brightness_str, color_str), 1);
}