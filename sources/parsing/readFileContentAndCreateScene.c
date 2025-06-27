#include "minirt.h"

//
static char	findType(char *ptr)
{
	if (!strcmp(ptr, AMBIENT_LIGHTNING))
		return (EALIGHT);
	if (!strcmp(ptr, CAMERA))
		return (ECAMERA);
	if (!strcmp(ptr, LIGHT))
		return (ELIGHT);
	if (!strcmp(ptr, SPHERE))
		return (ESPHERE);
	if (!strcmp(ptr, PLANE))
		return (EPLANE);
	if (!strcmp(ptr, CYLINDER))
		return (ECYLINDER);
    return (0);
}

//
static t_object	*parseLine(char *line)
{
	t_object	*new;
	char		*ptr;

	new = createObjectNode();
	if (!new)
		return (NULL);
	ptr = ft_strtok_r(line, WHITESPACES, &line);
	new->type = findType(ptr);
    if (!new->type || !fillObject(&new, line))
        new = freeObject(new);
    return (new);
}

//This helper returns the next line that is either NULL (EOF) or has some
//content. Empty lines are skipped recursively.
static char *getNextValidLine(int fd, char **saveptr)
{
    char    *line;

    line = get_next_line(fd);
    if (line && (!*line || *line == '\n' || !ft_strcmp(line, "")))
    {
        line = freeGenericPointer(line);
        return (getNextValidLine(fd, saveptr));
    }
    *saveptr = line;
    return (line);
}

//If the parsing was interrupted due to invalid, status, we return false.
//Else, we return true if the scene as a light source, a camera && ambiaent
//light settings.
static bool endOfParsing(t_minirt *minirt, bool status, char **saveptr)
{
    t_scene *scene;

    *saveptr = freeGenericPointer(*saveptr);
    if (!status)
        return (status);
    scene = minirt->scene;
    return (scene->light && scene->camera && scene->ambiantLightning);
}

//This function reads each line of the file and processes its content to turn
//it into a scene parameter or an object to be displayed.
bool    readFileContentAndCreateScene(t_minirt *minirt, int fd)
{
	t_object	*new;
	t_object	*tail;
	char		*line;
    char        *saveptr;
    bool        parsingStatus;

	tail = NULL;
    saveptr = NULL;
    parsingStatus = true;
	minirt->scene = createSceneNode();
	if (!minirt->scene)
		return (NULL);
	while (parsingStatus)
	{
        line = getNextValidLine(fd, &saveptr);
        if (!line)
            break ;
		new = parseLine(line);
		if (!new)
            break ;
        if (!insertNewObjectInMinirt(minirt, new, &tail))
            break ;
		saveptr = freeGenericPointer(saveptr);
	}
	return (close(fd), endOfParsing(minirt, parsingStatus, &saveptr));
}
