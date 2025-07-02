#ifndef STRUCTURES_H
# define STRUCTURES_H

//General defintions

//Main structure holding all the program's informations
typedef	struct	minirt
{
	int				width;
	int				height;
	void    	    *mlxptr;
	void	        *mlxwin;
    struct scene    *scene;
}	t_minirt;

//Object specific structure holding a pointer to unique objects light, camera and
//ambiant lighting. Also holds a linked list of all objects present in the scen
typedef struct	scene
{
    struct object	*light;
    struct object	*camera;
    struct object	*objects;
    struct object	*ambiantLightning;
}	t_scene;

/* OLD DEFINTION
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
*/

typedef struct vector
{
	float	x;
	float	y;
	float	z;
}	t_vec;

typedef struct	triangle
{
	struct vector	v1;
	struct vector	v2;
	struct vector	v3;
	struct triangle	*next;
}	t_triangle;


typedef struct meshes
{
	struct meshes	*next;
}	t_mesh;

//BVH defitions

//Volume boundaries node
typedef struct aabb
{
	t_vec	min_vec;
	t_vec	max_vec;
}	t_aabb;


//BVH tree structure
typedef struct bvh
{
	int				objCount;
	struct object	*objects;
    struct aabb		*bounds;
    struct bvh		*left;
    struct bvh		*right;
}	t_bvh;

//Bonus
typedef struct material
{
    float	albedo[4];
    float	refractive_index;
    float	specular_exponent;
    t_vec	diffuse_color;
}	t_material;

typedef struct hit
{
    bool		hit;
    float		distance;
    t_vec		point;
    t_vec		normal;
    t_material	material;
}	t_hit;


/*refactored structures*/

typedef struct	primitives
{
	int		color;
	t_vec	center;	
	t_vec	normalized_axis;	
	t_aabb	boundaries;
}	t_prim;

typedef struct methods
{
    void	*(*initializer)(char **, void *, t_prim *);
    bool	(*intersect)(struct object *, t_vec, t_vec, t_hit *);
    t_aabb	(*bounds)(struct object *);
    void 	(*destroy)(struct object *);
}	t_methods;

/*
typedef	struct	object
{
	char			type;
	union objtype
	{
        struct	sphere		sphere;
        struct	plane		plane;
        struct	cylinder	cylinder;
        struct	camera		camera;
        struct	light		light;
        struct	ambient		ambient;
    }	u_type;
	void			*data;
	t_prim			pdata;
	const t_methods	*methods;
	struct	object	*next;
}	t_object;
*/

typedef struct	camera
{
	float	fov;	
	t_vec	view_point;
	t_vec	normalized_orientation;
}	t_camera;

typedef struct	ambient
{
	int		color;
	float	light_ratio;
}	t_ambient;

typedef struct	light
{
	int		color;
	float	brightness;
	t_vec	light_position;
}	t_light;

typedef struct	sphere
{
	int		color;
	t_vec	center;
	t_vec	normalized_axis;	
	float	diameter;
}	t_sphere;

typedef struct	cylinder
{
	int		color;
	t_vec	center;	
	t_vec	normalized_axis;	
	float	height;
	float	diameter;
}	t_cylinder;

typedef struct	plane
{
	int		color;
	t_vec	center;
	t_vec	normalized_axis;
}	t_plane;


typedef	struct	object
{
	char			type;
	union objtype
	{
        struct	sphere		sphere;
        struct	plane		plane;
        struct	cylinder	cylinder;
        struct	camera		camera;
        struct	light		light;
        struct	ambient		ambient;
    }	u_type;
	void			*data;
	t_prim			pdata;
	const t_methods	*methods;
	struct	object	*next;
}	t_object;

#endif
