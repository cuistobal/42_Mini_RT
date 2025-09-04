/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 09:12:03 by chrleroy         ###   ########.fr       */
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

static inline void	set_parse_light(t_plight *plight, char **line)
{
	plight->position_str = get_next_token(line);
	plight->brightness_str = get_next_token(line);
	plight->color_str = get_next_token(line);
}

/*
** parse_light - Parse light element from line (L x,y,z brightness r,g,b)
** line: Line to parse
** scene: Scene structure to populate
** Returns: 1 on success, 0 on error
*/
int	parse_light(char *line, t_scene *scene)
{
	t_plight	pl;

	if (!line || !scene)
		return (0);
	set_parse_light(&pl, &line);
	if (!pl.position_str || !pl.brightness_str || !pl.color_str)
		return (0);
	if (!parse_vec3(pl.position_str, &pl.position)
		|| !parse_double(pl.brightness_str, &pl.brightness)
		|| !parse_color(pl.color_str, &pl.color))
		return (free_light_tokens(pl.position_str, pl.brightness_str, \
					pl.color_str), 0);
	if (pl.brightness < 0.0 || pl.brightness > 1.0)
		return (free_light_tokens(pl.position_str, pl.brightness_str, \
					pl.color_str), 0);
	// Possible leak here if parsing fails;
	pl.light = safe_malloc(sizeof(t_light));
	pl.light->position = pl.position;
	pl.light->intensity = pl.brightness;
	pl.light->color = pl.color;
	pl.light->next = NULL;
	add_light_to_scene(scene, pl.light);
	return (free_light_tokens(pl.position_str, pl.brightness_str, \
				pl.color_str), 1);
}
