#include "minirt.h"

//
static char	find_type(char *ptr)
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

//The shit we do to get OOP like behaviours when only pure C is allowed.
//Populates the methods struct with the object's type matching methods.
static void	get_methods(char type, t_methods *out)
{
	if (type == ESPHERE)
		*out = (t_methods){.initializer = sphere_initialiser, .intersect = \
			sphere_intersect, .bounds = sphere_bounds, .destroy = destroy};
	else if (type == EPLANE)
		*out = (t_methods){.initializer = plane_initialiser, .intersect = \
			plane_intersect, .bounds = no_bounds, .destroy = destroy};
	else if (type == ECYLINDER)
		*out = (t_methods){.initializer = cylinder_init, .intersect = \
			cylinder_intersect, .bounds = cylinder_bounds, .destroy = destroy};
	else if (type == ECAMERA)
		*out = (t_methods){.initializer = camera_initialiser, .intersect = \
			NULL, .bounds = no_bounds, .destroy = destroy};
	else if (type == ELIGHT)
		*out = (t_methods){.initializer = light_initialiser, .intersect = \
			NULL, .bounds = no_bounds, .destroy = destroy};
	else if (type == EALIGHT)
		*out = (t_methods){.initializer = ambient_initialiser, .intersect = \
			NULL, .bounds = no_bounds, .destroy = destroy};
	else
		*out = (t_methods){0};
}

//
static t_object	*parse_line(char *line)
{
	t_object	*new;
	char		*ptr;

	new = malloc(sizeof(t_object));
	if (!new)
		return (NULL);
	ptr = ft_strtok_r(line, WHITESPACES, &line);
	new->next = NULL;
	new->type = find_type(ptr);
	if (!new->type)
		new = free_object(new);
	get_methods(new->type, &new->methods);
	if (!call_object_initializer(new->type, &line, new))
		return (free_generic_pointer(new));
	return (new);
}

//This helper returns the next line that is either NULL (EOF) or has some
//content. Empty lines are skipped recursively.
static char *get_next_valid_line(int fd, char **saveptr)
{
	char    *line;

	line = get_next_line(fd);
	if (line && (!*line || *line == '\n' || !ft_strcmp(line, "")))
	{
		line = free_generic_pointer(line);
		return (get_next_valid_line(fd, saveptr));
	}
	*saveptr = line;
	return (line);
}

/*Can be removed
//If the parsing was interrupted due to invalid, status, we return false.
//Else, we return true if the scene as a light source, a camera && ambiaent
//light settings.
static bool endOfParsing(t_minirt *minirt, char **saveptr)
{
	t_scene *scene;

	scene = minirt->scene;
	*saveptr = freeGenericPointer(*saveptr);
	return (scene->light && scene->camera && scene->ambiantLightning);
}
*/

//This function reads each line of the file and processes its content to turn
//it into a scene parameter or an object to be displayed.
bool    read_file_content_and_create_scene(t_minirt *minirt, int fd)
{
	t_object	*new;
	t_object	*tail;
	char		*line;
	t_scene		*scene;
	char        *saveptr;

	tail = NULL;
	saveptr = NULL;
	scene = create_scene_node();
	if (!scene)
		return (false);
	minirt->scene = scene;
	while (true)
	{
		line = get_next_valid_line(fd, &saveptr);
		if (!line)
			break ;
		new = parse_line(line);
		if (!new)
			break ;
		if (!insert_new_object_in_minirt(minirt, new, &tail))
			break ;
		saveptr = free_generic_pointer(saveptr);
	}
//	return (close(fd), endOfParsing(minirt, &saveptr));
	return (close(fd), saveptr = free_generic_pointer(saveptr), scene->light && \
			scene->camera && scene->ambiant);
}
