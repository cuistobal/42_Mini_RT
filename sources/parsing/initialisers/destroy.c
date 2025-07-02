#include "minirt.h"

void	*destroy(t_object *self)
{
	if (self->data)
		self->data = freeGenericPointer(self->data);
	return (freeGenericPointer(self));
}
