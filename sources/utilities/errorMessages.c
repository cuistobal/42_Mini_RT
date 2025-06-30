#include "minirt.h"

//
bool	printErrorMessage(char *msg)
{
	return (write(2, msg, ft_strlen(msg)), false);
}
