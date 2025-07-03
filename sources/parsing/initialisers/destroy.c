#include "minirt.h"

void	*destroy(t_object *self)
{
	if (self->data)
		self->data = free_generic_pointer(self->data);
	return (free_generic_pointer(self));
}
