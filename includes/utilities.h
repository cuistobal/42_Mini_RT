#ifndef UTILITIES_H
# define UTILITIES_H

# include "libft.h"

//sources/utilities/errorMessages.c
bool	printErrorMessage(char *msg);

//sources/utilities/freeUtils.c
void    		*freeGenericPointer(void *ptr);
void			*freeArr(char **arr, int len);
void			*freeObject(t_object *ptr);
void			*freeScene(t_scene *ptr);
void			*freeMinirt(t_minirt *ptr);

//sources/utilities/createNodes.c
t_object    	*createObjectNode(void);
t_scene     	*createSceneNode(void);

//sources/utilities/my_atof.c
double  		ft_atof(const char *str, bool *error);

//sources/utilities/rgbConvert.c
bool    		get_colors(int *colot, char **str);
bool    		getColours(t_object *object, char **str);

//sources/utilities/init_window.c
void 			init_window(t_minirt *scene);

//sources/utilities/computing.c
inline float	findDistance(int *arr);
inline float	computeScalarProduct(int *obj1, int *obj2);

//sources/utilities/vectorOperations.c
t_vec	vec_add(t_vec a, t_vec b);
t_vec	vec_sub(t_vec a, t_vec b);
t_vec	vec_scale(t_vec vector, float s);
float	vec_dot(t_vec a, t_vec b);
float	vec_norm(t_vec v);


//sources/utilities/vectorOperations1.c
t_vec	vec_normalized(t_vec *v);
t_vec	vec_negate(t_vec v);
t_vec	vec_cross(t_vec a, t_vec b);
t_vec	set_vec_values(float x, float y, float z);

#endif
