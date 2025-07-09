#include "minirt.h"

//
int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;
	
	if (!parsing(argc, argv, envp, &minirt))
		return (1);

	start_rendering(&minirt);
	mlx_clear_window(minirt.screen.mlxptr, minirt.screen.mlxwin);
	mlx_destroy_display(minirt.screen.mlxptr);

	return (free_minirt(&minirt), 0);
}
