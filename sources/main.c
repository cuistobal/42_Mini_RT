#include "minirt.h"

//
int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;

	if (!parsing(argc, argv, envp, &minirt))
		return (print_minirt(minirt), 1);
	minirt.width = 800;
	minirt.height = 1200;
	start_rendering(&minirt);
	return (free_minirt(&minirt), 0);
}
