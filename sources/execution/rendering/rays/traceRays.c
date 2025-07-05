#include "minirt.h"

//
static t_vec	append_dir_value(t_minirt *minirt, int x, int y)
{
	t_vec	ret;
	float	fov;

	fov = minirt->scene->camera->u_type.camera.fov;
	ret.x = (x + 0.5f) - minirt->screen.width / 2.0f;
	ret.y = -(y + 0.5f) + minirt->screen.height / 2.0f;
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

//Protoype might evolve if we need to use a boolean return downstream.
static void	get_color_and_append_img(t_minirt *minirt, t_img *img, int x, int y)
{
	t_vec	ray;
	t_vec	dir;
	t_vec	ret;
	float	color;
	//int		offset;	

	(void)img;
	color = 0X000000;
	dir = append_dir_value(minirt, x, y);
	ray = minirt->scene->camera->u_type.camera.view_point;
	ret = cast_ray(minirt->scene, ray, dir, 0);
	if (ret.x == 0.2 && ret.y == 0.7 && ret.z == 0.8)	
		color = 0Xffffff;
/*
	offset = y * img->size_line + x * (img->bpp / 8);
	printf("%d\n", offset);	
    *(unsigned int *)(img->data + offset) = color.x;
    *(unsigned int *)(img->data + offset + 1) = color.y;
    *(unsigned int *)(img->data + offset + 2) = color.z;	
*/
	mlx_pixel_put(minirt->screen.mlxptr, minirt->screen.mlxwin, x, y, color);
}


//Protoype might evolve if we need to use a boolean return downstream.
void	trace_rays(t_minirt *minirt, int width, int height)
{
	int		x;
	int		y;
	t_img	*img;

	y = 0;
	img = (t_img *)minirt->screen.tmp_img;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			get_color_and_append_img(minirt, img, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(minirt->screen.mlxptr, minirt->screen.mlxwin, img, 0, 0);
}
