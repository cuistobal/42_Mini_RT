#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef	struct	minirt
{
	void    	    *mlxptr;
	void	        *mlxwin;
    struct scene    *scene;
}	t_minirt;

typedef struct	scene
{
    struct objects  *light;
    struct objects  *camera;
    struct objects  *objects;
    struct objects  *ambiantLightning;
}	t_scene;

typedef struct	objects
{
	char	        type;
    long            colours[4];
    float           ratio[2];
    float           coordinates[3];
    float           normVector[3];
    struct objects  *next;
}	t_object;

#endif
