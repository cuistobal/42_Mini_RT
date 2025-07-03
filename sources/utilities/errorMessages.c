#include "minirt.h"

//
bool	print_error_message(char *msg)
{
	return (write(2, msg, ft_strlen(msg)), false);
}
