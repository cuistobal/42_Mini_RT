#include "minirt.h"

int	main(int argc, char **argv, char **envp)
{
	int			fd;
	const char	*file;
	t_minirt	scene;

	if (!parsing(argc, argv, envp, &scene))
		return (1);
	openWIndow(*argv);
	return (free_minirt(&scene), 0);
}
