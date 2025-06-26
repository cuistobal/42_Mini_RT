#include "minirt.h"

//
static int	openFile(const char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		write(2, PERMISSIONS, strlen(PERMISSIONS));
	return (fd);
}

//
static const char	*initParsing(int argc, char **argv, char **envp)
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

//
t_minirt	*parsing(int argc, char **argv, char **envp)
{
	int			fd;
	t_minirt	*scene;
	const char	*filename;

	filename = initParsing(argv, argv + 1, envp);
	if (!file)
		return (NULL);
	fd = openFile(filename);
	if (fd < 0)
		return (NULL);
	return (readFileContentAndCreateScene(fd));
}
