#ifndef UTILITIES_H
# define UTILITIES_H

# include "libft.h"

//sources/utilities/freeUtils.c
void    *freeGenericPointer(void *ptr);
void	*freeArr(char **arr, int len);
void	*freeObject(t_object *ptr);
void	*freeScene(t_scene *ptr);
void	*freeMinirt(t_minirt *ptr);

//sources/utilities/createNodes.c
t_object    *createObjectNode(void);
t_scene     *createSceneNode(void);

//sources/utilities/my_atof.c
double  ft_atof(const char *str, bool *error);

//sources/utilities/rgbConvert.c
bool    getColours(t_object *object, char **str);

//sources/utilities/init_window.c
void init_window(t_minirt *scene);

#endif
