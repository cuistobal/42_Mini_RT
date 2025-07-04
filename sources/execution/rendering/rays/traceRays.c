#include "minirt.h"

//
static t_vec	append_dir_value(t_minirt *minirt, int i, int j)
{
	t_vec	ret;
	float	fov;

	fov = minirt->scene->camera->u_type.camera.fov;
	ret.x = (i + 0.5f) - minirt->screen.width / 2.0f;
	ret.y = -(j + 0.5f) + minirt->screen.height / 2.0f;
	ret.z = -minirt->screen.height / (2.0f * tanf(fov / 2.0f));
	return ((t_vec)vec_normalized(ret));
}

/*
static t_vec	*createFrameBuffer(t_minirt *minirt, int size)
{
	t_vec		*buffer;

	buffer = malloc(sizeof(t_vec) * size);
	if (buffer)
		return (NULL);
	return (buffer);
}
*/

//

//Protoype might evolve if we need to use a boolean return downstream.
static void	get_color_and_append_img(t_minirt *minirt, t_img *img, int x, int y)
{
	t_vec	ray;
	t_vec	dir;
	t_vec	res;
	float	color;
	int		offset;	

	dir = append_dir_value(minirt, x, y);
	ray = set_vec_value(0.0f, 0.0f, 0.0f);
	res = cast_ray(minirt->scene, ray, dir, 0);
	color = vec_norm(res);
	offset = y * img->size_line + x * (img->bpp / 8);
    *(unsigned int *)(img->data + offset) = color;
}


//Protoype might evolve if we need to use a boolean return downstream.
void	trace_rays(t_minirt *minirt, int width, int height)
{
	int		x;
	int		y;
	t_img	*img;

	x = 0;
	img = (t_img *)minirt->screen.tmp_img;
	while (x < height)
	{
		y = 0;
		while (y < width)
		{
			get_color_and_append_img(minirt, img, x, y);
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(minirt->screen.mlxptr, minirt->screen.mlxwin, img, 0, 0);
}
