#ifndef EXECUTION_H
# define EXECUTION_H

//Rendering

//sources/execution/rendering/startRendering.c
bool	startRendering(t_minirt *minirt);

//sources/execution/rendering/getSah.c
void    getSah(t_bvh *node, int *bestAxis, float *bestSplit);

//sources/execution/rendering/buildBVH.c
bool    buildBvh(t_bvh **root, t_object *objects, int count, int depth);

//sources/execution/rendering/aabbNodeHandler.c
float	getAabbSurfaceArea(t_aabb *boundaries);
void	combineAabbNodes(t_aabb *node1, t_aabb *node2);
t_aabb  *createAabbNode(t_object *object);

//
t_bvh	*buildBvhNode(t_object *objects, int start, int end, int depth);

#endif
