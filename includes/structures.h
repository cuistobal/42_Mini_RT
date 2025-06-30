#ifndef STRUCTURES_H
# define STRUCTURES_H

//General defintions

//Main structure holding all the program's informations
typedef	struct	minirt
{
	void    	    *mlxptr;
	void	        *mlxwin;
    struct scene    *scene;
}	t_minirt;

//Object specific structure holding a pointer to unique objects light, camera and
//ambiant lighting. Also holds a linked list of all objects present in the scen
typedef struct	scene
{
    struct objects  *light;
    struct objects  *camera;
    struct objects  *objects;
    struct objects  *ambiantLightning;
}	t_scene;

//Object nodes, contaning the type and metrics of objects. Some parameter's
//values stay set @ 0 if the object's type don't need them (ex -> We don't need
//colours for the ECAMARA type).
typedef struct	objects
{
	char	        type;
    long            colours[4];
    float           ratio[2];
    float           coordinates[3];
    float           normVector[3];
    struct objects  *next;
}	t_object;


//BVH defitions

//Volume boundaries node
typedef struct aabb
{
	int	minVec[3];
	int	maxVec[3];
}	t_aabb;

//BVH tree structure
typedef struct bvh
{
	int				objCount;
	struct objects	*objects;
    struct aabb		*bounds;
    struct bvh		*left;
    struct bvh		*right;
}	t_bvh;

#endif
