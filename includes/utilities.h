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
bool    		getColours(t_object *object, char **str);

//sources/utilities/init_window.c
void 			init_window(t_minirt *scene);

//sources/utilities/computing.c
inline float	findDistance(int *arr);
inline float	computeScalarProduct(int *obj1, int *obj2);

//sources/utilities/vectorOperations.c
inline t_vec	vecAdd(t_vec a, t_vec b);
inline t_vec	vecSub(t_vec a, t_vec b);
inline void		vecScale(t_vec *vector, float s);
inline float	vecDot(t_vec a, t_vec b);
inline float	vecNorm(t_vec v);


//sources/utilities/vectorOperations1.c
inline t_vec	vecNormalized(t_vec *v);
inline t_vec	vecNegate(t_vec v);
inline t_vec	vecCross(t_vec a, t_vec b);
inline t_vec	setVecValues(float x, float y, float z);

#endif
