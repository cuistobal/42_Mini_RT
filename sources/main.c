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
	int			fd;
	const char	*file;

	file = initParsing(argc, argv + 1, envp);
	if (!file)
		return (1);
	fd = openFile(file);
	if (fd < 0)
		return (fd);
	return (0);
}
