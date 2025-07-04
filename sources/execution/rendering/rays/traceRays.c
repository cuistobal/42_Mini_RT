#include "minirt.h"

//
static void	append_dir_value(t_minirt *minirt, t_vec *dir, int i, int j)
{
	float	fov;

	fov = minirt->scene->camera->u_type.camera.fov;
	dir->x = (i + 0.5f) - minirt->screen.width / 2.0f;
	dir->y = -(j + 0.5f) + minirt->screen.height / 2.0f;
	dir->z = -minirt->screen.height / (2.0f * tanf(fov / 2.0f));
	*dir = vec_normalized(*dir);	
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
void	trace_rays(t_minirt *minirt, int width, int height)
{
	int		i;
	int		j;
	t_vec	dir;
	t_vec	ray;
	t_vec	*frame_buffer;

	i = 0;
	j = 0;
	//frame_buffer = malloc(sizeof(t_vec) * minirt->screen.width * minirt->screen.height);
	frame_buffer = malloc(sizeof(t_vec) * width * height);
	if (!frame_buffer)
		return ;
	//while (i < minirt->height)
	while (i < height)
	{
		//while (j < minirt->width)
		while (j < width)
		{
			ray = set_vec_value(0.0f, 0.0f, 0.0f);	
			append_dir_value(minirt, &dir, i, j);
			frame_buffer[j * width + i] = cast_ray(minirt->scene, ray, dir, 0);	
			//frame_buffer[j * minirt->width + i] = cast_ray(minirt->scene, ray, dir, 0);	
			j++;
		}
		i++;
	}
	for (int i = 0; i < (width * height); i++)
		print_vec(ft_itoa(i), frame_buffer[i]);
}
