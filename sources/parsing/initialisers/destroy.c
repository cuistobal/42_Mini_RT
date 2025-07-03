#include "minirt.h"

void	*destroy(t_object *self)
{
	return (free_generic_pointer(self));
}
