/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vec3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 08:41:33 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 08:48:09 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** parse_vec3 - Parse a 3D vector from string (format: x,y,z)
** str: String to parse
** result: Pointer to store result
** Returns: 1 on success, 0 on error
*/
int	parse_vec3(char *str, t_vec3 *result)
{
	char	*str_copy;
	char	*token;
	char	*next;
	double	values[3];
	int		i;

	if (!str || !result)
		return (0);
	str_copy = safe_strdup(str);
	str_copy = skip_whitespace(str_copy);
	i = 0;
	token = str_copy;
	while (i < 3 && token)
	{
		next = strchr(token, ',');
		if (next)
			*next = '\0';
		if (!parse_double(token, &values[i]))
			return (safe_free((void **)&str_copy), 0);
		if (next)
			token = next + 1;
		else
			token = NULL;
		i++;
	}
	safe_free((void **)&str_copy);
	if (i != 3)
		return (0);
	result->x = values[0];
	result->y = values[1];
	result->z = values[2];
	return (1);
}
