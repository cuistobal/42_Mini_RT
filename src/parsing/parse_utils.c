/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** skip_whitespace - Skip whitespace characters in a string
** str: String to process
** Returns: Pointer to first non-whitespace character
*/
static char	*skip_whitespace(char *str)
{
	if (!str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r'))
		str++;
	return (str);
}

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
	if (endptr == str || (*endptr != '\0' && *endptr != ' ' && 
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
	if (endptr == str || (*endptr != '\0' && *endptr != ' ' && 
		*endptr != '\t' && *endptr != ',' && *endptr != '\n'))
		return (0);
	if (value < INT_MIN || value > INT_MAX)
		return (0);
	*result = (int)value;
	return (1);
}/*
** par
se_vec3 - Parse a 3D vector from string (format: x,y,z)
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
	char	*next;
	int		values[3];
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
		if (!parse_int(token, &values[i]))
			return (safe_free((void **)&str_copy), 0);
		if (values[i] < 0 || values[i] > 255)
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
	result->r = values[0];
	result->g = values[1];
	result->b = values[2];
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