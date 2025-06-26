#include "minirt.h"

int	limited_char(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
	{
		if (!ft_isdigit(str[i]) && str[i] != ',')
			return (-1);
		i++;
	}
	return (1);
}

int	check_rgb_commat(const char *str)
{
	int	i;
	int	commat;

	i = 0;
	commat = 0;
	while (str[i])
	{
		if (str[i] == ',')
			return (-1);
		while (str[i] && str[i] != ',')
			i++;
		if (str[i] == ',')
		{
			commat++;
			i++;
		}
	}
	if (commat != 2 || str[i - 1] == ',')
		return (-1);
	return (1);
}

int	check_error_char_rgb(const char *str)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (str[i] != '\n' && str[i] != '\0')
	{
		if (ft_isdigit(str[i]) != 1 && str[i] != ',')
			return (-1);
		if (ft_isdigit(str[i]) == 1)
			n++;
		else
		{
			if (n > 3)
				return (-1);
			n = 0;
		}
		i++;
	}
	if (n == 0)
		return (-1);
	return (1);
}

int	convert_rgb_atoi(char *rgb)
{
	int	red;
	int	green;
	int	blue;

	red = ft_atoi(rgb);
	if (red > 255 || red < 0)
		return (-1);
	red = red * 65536;
	while (*rgb != ',')
		rgb++;
	green = ft_atoi(++rgb);
	if (green > 255 || green < 0)
		return (-1);
	green = green * 256;
	while (*rgb != ',')
		rgb++;
	blue = ft_atoi(++rgb);
	if (blue > 255 || blue < 0)
		return (-1);
	return (red | green | blue);
}

int	is_valid_rgb(t_stock *intel, char *rgb)
{
	int	color;

	if (check_rgb_commat(rgb) == -1)
		error_game(intel, "Error format color");
	if (check_error_char_rgb(rgb) == -1)
		error_game(intel, "Error format color");
	if (limited_char(rgb) == -1)
		error_game(intel, "Error format color");
	color = convert_rgb_atoi(rgb);
	if (color == -1)
		error_game(intel, "Error format color");
	return (color);
}
