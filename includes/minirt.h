#ifndef MINIRT_H
# define MINIRT_H

# include <math.h>
# include <math.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "errors.h"
# include "structures.h"
# include "globalVariables.h"
# include "utilities.h"
# include "temp.h" //includes for forbidden && test functions
# include "parsing.h"
# include "execution.h"
# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"

int	main(int argc, char **argv, char **envp);

#endif
