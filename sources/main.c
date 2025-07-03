#include "minirt.h"

//
int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;

	if (!parsing(argc, argv, envp, &minirt))
		return (printf("aled\n")); 
//		return (printMinirt(minirt), 1);
	//printMinirt(minirt);
//    init_window(&minirt);
	startRendering(&minirt);
	return (free_minirt(&minirt), 0);
}
