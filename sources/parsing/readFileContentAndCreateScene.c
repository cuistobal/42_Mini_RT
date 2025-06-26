#include "minirt.h"

static char	findType(char *ptr)
{
	if (!strcmp(ptr, AMBIENT_LIGHTNING))
		return (65);
	if (!strcmp(ptr, CAMERA))
		return (66);
	if (!strcmp(ptr, LIGHT))
		return (67);
	if (!strcmp(ptr, SPHERE))
		return (68);
	if (!strcmp(ptr, PLANE))
		return (69);
	if (!strcmp(ptr, CYLINDER))
		return (70);
}

//
static t_object	parseLine(char *line)
{
	t_object	*new;
	char		*src;
	char		*ptr;

	new = createObjectNode();
	if (!new)
		return (NULL);
	src = line;	
	ptr = ft_strtok_r(line, WHITESPACES, &line);
	new->type = findType(ptr);
	fillObject(&new, line);
	new->color = findColor(line);
/*
	int			len;
	char		**charsplit;
	
	len = 0;
	split = splitLine(line, WHITESPACES, &len);
	if (!split)
		return (freeObject(new));
	fillObject(&new, split, len);/
	return (freeArr(split, len), new);
*/
	return (free(src), src = NULL, new);
}

//
t_minirt	*readFileContentAndCreateScene(int fd)
{
	t_object	*new;
	t_object	*tail;
	t_minirt	*minirt;
	char		*element;

	head = NULL;
	tail = NULL;
	minirt->scene = createMinirtNode();
	if (!scene)
		return (NULL);
	element = get_next_line(fd);
	while (element)
	{
		new = parseLine(element);
		if (!new)
			return (close(fd), freeMinirt(scene), scene);	
		insertNewObject(&minirt->scene->objects, &tail, new);
		free(element);
		element = get_next_line(fd);
	}
	return (close(fd), minirt);
}

