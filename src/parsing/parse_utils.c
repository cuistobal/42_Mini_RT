/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/30 08:41:58 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** parse_double - Parse a double from string
** str: String to parse
** result: Pointer to store result
** Returns: 1 on success, 0 on error
*/
int	parse_double(char *str, double *result)
{
	char	*endptr;
	double	value;

	if (!str || !result)
		return (0);
	str = skip_whitespace(str);
	if (*str == '\0')
		return (0);
	value = strtod(str, &endptr);
	if (endptr == str || (*endptr != '\0' && *endptr != ' ' && \
				*endptr != '\t' && *endptr != ',' && *endptr != '\n'))
		return (0);
	*result = value;
	return (1);
}

/*
** parse_int - Parse an integer from string
** str: String to parse
** result: Pointer to store result
** Returns: 1 on success, 0 on error
*/
int	parse_int(char *str, int *result)
{
	char	*endptr;
	long	value;

	if (!str || !result)
		return (0);
	str = skip_whitespace(str);
	if (*str == '\0')
		return (0);
	value = strtol(str, &endptr, 10);
	if (endptr == str || (*endptr != '\0' && *endptr != ' ' && \
				*endptr != '\t' && *endptr != ',' && *endptr != '\n'))
		return (0);
	if (value < INT_MIN || value > INT_MAX)
		return (0);
	*result = (int)value;
	return (1);
}

/*
** get_next_token - Get next token from string separated by spaces
** str: String to tokenize
** Returns: Pointer to next token, NULL if no more tokens
*/
char	*get_next_token(char **str)
{
	char	*token;
	char	*start;

	if (!str || !*str)
		return (NULL);
	*str = skip_whitespace(*str);
	if (**str == '\0')
		return (NULL);
	start = *str;
	while (**str && **str != ' ' && **str != '\t' && **str != '\n')
		(*str)++;
	if (**str)
	{
		**str = '\0';
		(*str)++;
	}
	token = safe_strdup(start);
	return (token);
}
