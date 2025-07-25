#ifndef PARSING_H
# define PARSING_H

//sources/parsing/insertNewObjectInMinirt.c
bool    	insert_new_object_in_minirt(t_minirt *minirt, t_object *new, t_object **tail);

bool    	fill_object(t_object *object, char *str);
bool    	read_file_content_and_create_scene(t_minirt *minirt, int fd);
bool    	parsing(int argc, char **argv, char **envp, t_minirt *minirt);

//sources/parsing/primitive_objects_initialisers.c
void		*sphere_initialiser(char **input, void *data, t_prim *pdata);
void		*plane_initialiser(char **input, void *data, t_prim *pdata);
void		*cylinder_init(char **input, void *data, t_prim *pdata);

//sources/parsing/environement_objects_initialisers.c
void		*camera_initialiser(char **input, void *data, t_prim *pdata);
void		*ambient_initialiser(char **str, void *data, t_prim *pdata);
void		*light_initialiser(char **input, void *data, t_prim *pdata);

//sources/parsing/helpers_for_initialisers.c
bool    	convert(float *coordinates, char *str);
bool    	checkLimits(bool status, float ratio, float min, float max);
void	*end_of_scenario(char *str, void *object);
bool		extract_float_from_string(float *value, char **str, \
		bool (*convert)(float *dst, char *src), float limits[]);
bool		extract_vector_from_string(t_vec *v, char **str, \
		bool (*convert)(float *dst, char *src), float limits[]);
void	*call_object_initializer(char type, char **line, t_object *obj);

//sources/parsing/initialisers/boundaries.c
t_aabb	populate_sphere_aabb(t_object *obj);
t_aabb	populate_plan_aabb(t_object *obj);
t_aabb	no_bounds(t_object *obj);

//void	populate_cylinder_aabb(t_object *obj)
t_aabb	sphere_bounds(t_object *obj);
t_aabb	no_bounds(t_object *obj);
t_aabb	cylinder_bounds(t_object *obj);

//sources/parsing/initialisers/destroy.c 
void	*destroy(t_object *self);

//sources/parsing/initialisers/intersections.c
bool	plane_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit);
bool	sphere_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit);
bool	cylinder_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit);

//sources/parsing/initialisers/intersections/cylinder_intersect.c
bool    cylinder_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit);

//sources/parsing/initialisers/intersections/plane_intersect.c
bool    plane_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit);

//sources/parsing/initialisers/intersections/sphere_intersect.c
bool    sphere_intersect(t_object *obj, t_vec orig, t_vec dir, t_hit *hit);

//sources/parsing/initialisers/materials.c
void    set_standard_material(t_material *m);

#endif
