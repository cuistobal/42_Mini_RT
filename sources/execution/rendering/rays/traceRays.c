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
static float clamp(float x)
{
	if (x < 0.0f)
		return (0.0f);
	else if (x > 1.0f)
		return (1.0f);
    return (x);
}

// Convert t_vec (with .x, .y, .z in [0,1]) to 0xRRGGBB
int vec_to_color(t_vec color)
{
	int	r;
	int	g;
	int b;

    r = (int)(255.0f * clamp(color.x));
    g = (int)(255.0f * clamp(color.y));
    b = (int)(255.0f * clamp(color.z));
	return ((r << 16) | (g << 8) | b);
}
*/

//Protoype might evolve if we need to use a boolean return downstream.
static void	get_color_and_append_img(t_minirt *minirt, t_img *img, int x, int y)
{
	t_vec	ray;
	t_vec	dir;
	int		color;
	int		offset;	

	dir = append_dir_value(minirt, x, y);
	ray = minirt->scene->camera->u_type.camera.view_point;
	color = cast_ray(minirt->scene, ray, dir, 0);
	offset = y * img->size_line + x * (img->bpp / 8);
    *(unsigned int *)(img->data + offset) = color;
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
}
