#include "minirt.h"

int	main(int argc, char **argv, char **envp)
{
	if (!envp || !*envp || argc != 2)
		return (1);
	*argv = NULL;
	return (0);
}
