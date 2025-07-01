#include "minirt.h"

//
static void	appendDirValue(t_minirt *minirt, t_vec *dir, int i, int j)
{
	dir->x = (i + 0.5f) - minirt->width / 2.0f;
	dir->y = -(j + 0.5f) + minirt->height / 2.0f;
	dir->z = -minirt->height / (2.0f * tanf(minirt->scene->camera->fov / 2.0f));
	vecNormalized(dir);	
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
void	traceRays(t_minirt *minirt)
{
	int		i;
	int		j;
	int		max;
	t_vec	dir;
	t_vec	ray;
	t_vec	*frameBuffer;

	max = minirt->width * minirt->height;
	framebuffer = malloc(sizeof(t_vec) * size);
	if (!framebuffer)
		return ;
	while (i < minirt->height)
	{
		while (j < minirt->width)
		{
			ray = setVecValues(0.0f, 0.0f, 0.0f);	
			dir = appendDirValue(minirt, &dir, i, j);
			frameBuffer[j * minirt->width + i] = castRay(scene, ray, dir, 0);	
			j++;
		}
		i++;
	}
}
