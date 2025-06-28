#ifndef UTILITIES_H
# define UTILITIES_H

# include "libft.h"

//sources/utilities/freeUtils.c
void	*freeArr(char **arr, int len);
void	*freeObject(t_object *ptr);
void	*freeScene(t_scene *ptr);
void	*freeMinirt(t_minirt *ptr);

//sources/utilities/createNodes.c
void	insertNewObject(t_object **head, t_object **tail, t_object *new);
void	*createObjectNode(void);
void	*createMinirtNode(void);

//sources/utilities/init_window.c
void init_window(t_minirt *scene);
#endif
