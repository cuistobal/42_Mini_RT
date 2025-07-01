#include "minirt.h"

void	traceRays(t_minirt *minirt)
{
	int		i;
	int		max;
	t_ray	*ray;

	max = getMaxPixels(minirt);
	while (i < max)
	{
		ray = createNewRay();
		if (intersects(ray))
		{
			addPixel();
			
		}
		i++;
	}
}
