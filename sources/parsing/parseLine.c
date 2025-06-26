#include "minirt.h"

bool	is_space(char c)
{
	return (c == 32 || c == 9 || c == 11);
}

void	skipSpaces(char *line, int *i)
{
	while (line[*i] && is_space(line[*i]))
		(*i)++;
}

void	parseLine(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		//if (!is_space(line[i]))

		i++;
	}
}
