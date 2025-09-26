/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 07:17:24 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/26 08:03:44 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	get_tokens(char **line, char *tokens[], int tcount)
{
	int	i;

	i = 0;
	while (i < tcount)
	{
		tokens[i] = get_next_token(line);
		if (!tokens[i])
			return (0);
		i++;
	}
	return (1);
}

int	get_material_tokens(char **line, char *tokens[], int tcount)
{
	int	i;

	i = 0;
	while (i < tcount)
	{
		if (!*line)
			tokens[i] = NULL;
		else
			tokens[i] = get_next_token(line);
		i++;
	}
	return (1);
}

void	free_tokens(char *tokens[], int tcount)
{
	int	i;

	i = 0;
	while (i < tcount)
	{
		safe_free((void **)&tokens[i]);
		i++;
	}
}

int	error_helper(t_object *obj, char *tokens[], int tcount)
{
	safe_free((void **)&obj);
	free_tokens(tokens, tcount);
	return (0);
}
