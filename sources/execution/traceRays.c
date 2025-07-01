#include "minirt.h"

//
static void	appendDirValue(t_minirt *minirt, t_vec *dir, int i, int j)
{
	dir->x = (i + 0.5f) - minirt->width / 2.0f;
	dir->y = -(j + 0.5f) + minirt->height / 2.0f;
	dir->z = -minirt->height / (2.0f * tanf(minirt->scene->camera->fov / 2.0f));
	vecNormalized(dir);	
}

//
static t_vec	*createFrameBuffer(t_minirt *minirt, int size)
{
	t_vec		*buffer;

	buffer = malloc(sizeof(t_vec) * size);
	if (buffer)
		return (NULL);
	while (size--)
	{
		buffer[size].x = 0;
		buffer[size].y = 0;
		buffer[size].z = 0;
	}
	return (buffer);
}

void	castRay(t_scene *scene, t_vec orig, t_vec dir, int depth)
{
	if (depth > 4)
	{
		
	}

}
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
	frameBuffer = createFrameBuffer(minirt, max);
	while (i < minirt->height)
	{
		while (j < minirt->width)
		{
			setEmptyVec(&ray);	
			dir = appendDirValue(minirt, &dir, i, j);
			frameBuffer[j * minirt->width + i] = castRay(scene, ray, dir, 0);	
			j++;
		}
		i++;
	}
}
