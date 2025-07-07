#include "minirt.h"

//
static void	*create_temp_image(t_screen	*screen)
{	
	int		width;
	int		height;
	void	*mlxptr;

	width = screen->width;
	height = screen->height;
	mlxptr = screen->mlxptr;
	if (screen->tmp_img)
		screen->tmp_img = purge_image(screen);
	screen->tmp_img = mlx_new_image(mlxptr, width, height);
	return (screen->tmp_img);
}

//
bool	start_rendering(t_minirt *minirt)
{
	t_bvh		*root;
	t_screen	*screen;
	t_object	*current;

	root = NULL;
	screen = &minirt->screen;
	current = minirt->scene->objects;
//Include a loop for frames here -> change trace_ray's return value;		
//The idea here is to loop permanently to create frame and render them in real
//time.
//Not important for the mandatory part.
	while (true)
	{
		if (!create_temp_image(screen))
			return (false);
		trace_rays(minirt, screen->width, screen->height);
		mlx_put_image_to_window(screen->mlxptr, screen->mlxwin, screen->tmp_img, 0, 0);
		screen->tmp_img = purge_image(screen);
	}	
	return (true);
}
