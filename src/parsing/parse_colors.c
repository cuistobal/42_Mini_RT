/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 08:41:20 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/30 08:47:18 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static inline int	loop_helper(char **token, int *value)
{
	char	*next;

	next = strchr(*token, ',');
	if (next)
		*next = '\0';
	if (!parse_int(*token, value))
		return (1);
	if (*value < 0 || *value > 255)
		return (1);
	if (next)
		*token = next + 1;
	else
		*token = NULL;
	return (0);
}

/*
** parse_color - Parse RGB color from string (format: r,g,b)
** str: String to parse
** result: Pointer to store result
** Returns: 1 on success, 0 on error
*/
int	parse_color(char *str, t_color *result)
{
	char	*str_copy;
	char	*token;
	int		values[3];
	int		i;

	if (!str || !result)
		return (0);
	i = 0;
	str_copy = safe_strdup(str);
	str_copy = skip_whitespace(str_copy);
	token = str_copy;
	while (i < 3 && token)
	{
		if (loop_helper(&token, &values[i]))
			return (safe_free((void **)&str_copy), 0);
		i++;
	}
	safe_free((void **)&str_copy);
	if (i != 3)
		return (0);
	result->r = values[0];
	result->g = values[1];
	result->b = values[2];
	return (1);
}

