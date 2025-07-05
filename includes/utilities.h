#ifndef UTILITIES_H
# define UTILITIES_H

# include "libft.h"

//sources/utilities/errorMessages.c
bool		print_error_message(char *msg);

//sources/utilities/freeUtils.c
void    	*free_generic_pointer(void *ptr);
void		*free_arr(char **arr, int len);
void		*free_object(t_object *ptr);
void		*free_scene(t_scene *ptr);
void		*free_minirt(t_minirt *ptr);

//sources/utilities/createNodes.c
t_object	*create_object_node(void);
t_scene     *create_scene_node(void);

//sources/utilities/my_atof.c
//double  	ft_atof(const char *str, bool *error);
float	ft_atof(const char *str, bool *error);

//sources/utilities/rgbConvert.c
bool    	get_colors(int *colot, char **str);

//sources/utilities/init_window.c
bool	init_window(t_screen *screen);

//sources/utilities/vectorOperations.c
t_vec		vec_add(t_vec a, t_vec b);
t_vec		vec_sub(t_vec a, t_vec b);
t_vec		vec_scale(t_vec vector, float s);
float		vec_dot(t_vec a, t_vec b);
float		vec_norm(t_vec v);


//sources/utilities/vectorOperations1.c
t_vec		vec_normalized(t_vec v);
t_vec		vec_negate(t_vec v);
t_vec		vec_cross(t_vec a, t_vec b);
t_vec		set_vec_value(float x, float y, float z);

#endif
