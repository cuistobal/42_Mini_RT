#include "minirt.h"

//Contains all the debug functions

void	print_vec(const char *label, t_vec v)
{
	printf("%s:	", label);
	printf("%.2f, ", v.x);
	printf("%.2f, ", v.y);
	printf("%.2f)\n", v.z);
}
/*
static void	print_pdata(const t_prim pdata)
{
	
}
*/
static void print_sphere(const t_sphere *s)
{
	printf("[SPHERE]:\n");
	printf("diameter: %.2f\n", s->diameter);
}
/*
static void print_plane(const t_plane *p)
{
	printf("[PLANE]:\n");
	
}
*/

static void print_cylinder(const t_cylinder *c)
{
	printf("[CYLINDER]:\n");
	printf("diameter: %.2f\n", c->diameter);
	printf("height:  %.2f\n", c->height);
}

static void print_camera(const t_camera *c)
{
	printf("[CAMERA]:\n");
	printf("fov:     %.2f\n", c->fov);
	print_vec("view_point", c->view_point);
	print_vec("orientation", c->normalized_orientation);
}

static void print_light(const t_light *l)
{
	printf("[LIGHT]:\n");
	printf("brightness: %.2f\n", l->brightness);
	print_vec("position", l->light_position);
}

static void print_ambient(const t_ambient *a)
{
	printf("[AMBIENT]:\n");
	printf("light_ratio: %.2f\n", a->light_ratio);
}

void	print_object(const t_object *obj)
{
	if (!obj)
		return;
	switch (obj->type)
	{
		case 'A': print_ambient(&obj->u_type.ambient); break;
		case 'B': print_camera(&obj->u_type.camera); break;
		case 'C': print_light(&obj->u_type.light); break;
		case 'D': print_sphere(&obj->u_type.sphere); break;
	//	case 'E': print_plane(&obj->u_type.plane); break;
		case 'F': print_cylinder(&obj->u_type.cylinder); break;
		default: printf("[UNKNOWN OBJECT TYPE %c]\n", obj->type); break;
	}
}

void	print_objects(t_object *objects)
{
	while (objects)
	{
		print_object(objects);
		objects = objects->next;
	}
	printf("\n");
}

void	print_minirt(const t_minirt minirt)
{
	if (!minirt.scene)
		return ;
	if (minirt.scene->camera)
		print_object(minirt.scene->camera);
	if (minirt.scene->light)
		print_object(minirt.scene->light);
	if (minirt.scene->ambiant)
		print_object(minirt.scene->ambiant);
	print_objects(minirt.scene->objects);
}

void	print_tree_bfs(const t_bvh *root)
{
	if (!root)
		return ;
	print_tree_bfs(root->left);
	print_tree_bfs(root->right);
	print_objects(root->objects);
}
