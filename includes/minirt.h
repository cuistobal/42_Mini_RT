#ifndef MINIRT_H
# define MINIRT_H

# include "temp.h" //includes for forbidden functions

# include <fcntl.h>
# include <math.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include "errors.h"
# include "globalVariables.h"
# include "utilities.h"
# include "parsing.h"
# include "execution.h"
# include "minilibx-linux/mlx.h"

typedef	struct	minirt
{
	void	*scene;
	void	*mlxptr;
	void	*mlxwin;
	void	*objects;

}	t_minirt;

typedef struct	scene
{

	void	*objects;
}	t_scene;

typedef struct	objects
{
	char	type;
	void	*next;
}	t_objects;

int	main(int argc, char **argv, char **envp);

#endif
