#ifndef UTILITIES_H
# define UTILITIES_H

# include "libft.h"

//sources/utilities/freeUtils.c
void    *freeStringSetNull(char *string);
void	*freeArr(char **arr, int len);
void	*freeObject(t_object *ptr);
void	*freeScene(t_scene *ptr);
void	*freeMinirt(t_minirt *ptr);

//sources/utilities/createNodes.c
//void	insertNewObject(t_object **head, t_object **tail, t_object *new);
void	*createObjectNode(void);
void	*createMinirtNode(void);

//sources/utilities/my_atof.c
double  ft_atof(const char *str);

//sources/utilities/rgbConvert.c
bool    getColours(t_object **object, char *str);

#endif
