#include "minirt.h"

int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;

	if (!parsing(argc, argv, envp, &minirt))
		return (1);
	return (freeMinirt(&minirt), 0);
}
