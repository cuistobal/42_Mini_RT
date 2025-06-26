#include "minirt.h"

void	*openWIndow()
{
    void    *mlx_ptr;
    void    *win_ptr;

    mlx_ptr = mlx_init();
    if (!mlx_ptr)
        return NULL;

    win_ptr = mlx_new_window(mlx_ptr, 800, 600, "My MiniRT");
    if (!win_ptr)
        return NULL;

    mlx_loop(mlx_ptr);

    return (0);
}

int	main(int argc, char **argv, char **envp)
{

	if (!envp || !*envp || argc != 2)
		return (1);
	*argv = NULL;
	return (0);
}
