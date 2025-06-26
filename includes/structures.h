#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef	struct	minirt
{
	void	*scene;
	void	*mlxptr;
	void	*mlxwin;
	void	*objects;

}	t_minirt;

typedef struct	scene
{

	void	*objects;
}	t_scene;

typedef struct	objects
{
	char	type;
	void	*next;
}	t_objects;

typedef struct vector
{
	int		size;
	int		type;
	int		capacity;
	void	*data;
}	t_vector;

#endif
