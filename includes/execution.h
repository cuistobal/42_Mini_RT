#ifndef EXECUTION_H
# define EXECUTION_H

//Rendering

//sources/execution/rendering/startRendering.c
bool	start_rendering(t_minirt *minirt);

//sources/execution/rendering/getSah.c
void    get_sah(t_bvh *node, int *bestAxis, float *bestSplit);

//sources/execution/rendering/buildBVH.c
bool    build_bvh(t_bvh **root, t_object *objects, int count, int depth);

//sources/execution/rendering/aabbNodeHandler.c
float	get_aabb_surface_area(t_aabb boundaries);
void	turn_vectors_to_aabb(t_aabb *node, t_vec v1, t_vec v2);
//void	combine_aabb_nodes(t_aabb *node1, t_aabb *node2);
t_aabb	combine_aabb_nodes(t_aabb node1, t_aabb node2);
t_aabb	set_aabb_value(t_vec min_vec, t_vec max_vec);
t_aabb  *create_aabb_node(t_object *object);

//
t_bvh	*build_bvh_node(t_object *objects, int start, int end, int depth);

//sources/execution/castRay.c
t_vec	cast_ray(t_scene *scene, t_vec orig, t_vec dir, int depth);

//sources/execution/reflexion_and_refraction.c
t_vec	reflect(t_vec a, t_vec b);
t_vec	refract(t_vec v1, t_vec v2, float v1eta, float v2eta);

//sources/execution/traceRays.c 
void	trace_rays(t_minirt *minirt, int width, int height);

//sources/execution/intersections.c
bool	scene_intersect(t_scene *scene, t_vec orig, t_vec dir, \
		t_hit *closestHit);

//sources/execution/rendering/init_window.c
bool	init_window(t_screen *screen);

#endif
