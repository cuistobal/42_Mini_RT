#include "minirt.h"

//
int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;

	if (!parsing(argc, argv, envp, &minirt))
		return (print_minirt(minirt), 1);
	init_window(&minirt.screen);
	start_rendering(&minirt);
	return (free_minirt(&minirt), 0);
}
