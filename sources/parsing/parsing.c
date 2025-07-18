#include "minirt.h"

//
static int	open_file(const char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		write(2, PERMISSIONS, strlen(PERMISSIONS));
	return (fd);
}

//
static const char	*init_parsing(int argc, char **argv, char **envp)
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
bool    parsing(int argc, char **argv, char **envp, t_minirt *minirt)
{
	int			fd;
	const char	*filename;
	
	filename = init_parsing(argc, argv + 1, envp);
	if (!filename)
		return (false);
	fd = open_file(filename);
	if (fd < 0)
		return (false);
    minirt->scene = NULL;
	minirt->screen.mlxptr = NULL;
    minirt->screen.mlxwin = NULL;
	minirt->screen.tmp_img = NULL;
	minirt->rendering.lru = NULL;
	minirt->rendering.root = NULL;
	if (read_file_content_and_create_scene(minirt, fd))
		return (init_window(&minirt->screen));
	return (false);
}
