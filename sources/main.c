#include "minirt.h"

//
int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;

	if (!parsing(argc, argv, envp, &minirt))
		return (1); 
	printMinirt(minirt);
//    init_window(&minirt);
	startRendering(&minirt);
	return (freeMinirt(&minirt), 0);
}
