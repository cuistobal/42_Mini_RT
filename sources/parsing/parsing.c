#include "minirt.h"

bool	readContentAndCreateScene(t_scene *scene, int fd)
{
	char	*element;

	element = getNextLine(fd);
	if (!element)
		return (close(fd), scene);
	parseLine()
}

int		openFile(const char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		write(2, PERMISSIONS, strlen(PERMISSIONS));
	return (fd);
}

const char	*initParsing(int argc, char **argv, char **envp)
{
	int			len;

	if (!envp || !*envp)
		return	(write(2, ENVIRON, strlen(ENVIRON)), NULL);
	if (argc != 2)
		return (write(2, USAGE, strlen(USAGE)), NULL);
	len = strlen(*argv);
	if (len < 3 || strcmp(*argv + len - 3, EXTENSION))
		return (write(2, FILENAME, strlen(FILENAME)), NULL);
	return (*argv);
}
