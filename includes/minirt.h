/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/08/21 14:01:59 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <string.h>
# include <limits.h>
# include <stdint.h>
# include "../minilibx-linux/mlx.h"
# include <pthread.h>


/* Constants */
# define EPSILON 1e-6
# define MAX_KEYS 256
# define CAPACITY_THRESHOLD 225 

/* Object types */
# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4
# define CUBE 5
# define TRIANGLE 6

/* Key codes */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

typedef enum e_keyaction {
    KEY_ACTION_W,
    KEY_ACTION_S,
    KEY_ACTION_A,
    KEY_ACTION_D,
    KEY_ACTION_UP,
    KEY_ACTION_DOWN,
    KEY_ACTION_LEFT,
    KEY_ACTION_RIGHT,
    KEY_ACTION_ESC,
    KEY_ACTION_COUNT
} t_keyaction;

# define RED_OFFSET 16
# define GREEN_OFFSET 8
# define BVH_STACK_SIZE 64
# define MAX_OBJECTS_PER_LEAF 4

// A modifier, etant donne que MAX_DIRS depend de la resolution de la fenetre et
// du nombre de threads
# define MAX_DIRS 3600

# ifndef NUM_THREAD
# 	define NUM_THREAD 64
# endif

# ifndef MAX_DEPTH
#  define MAX_DEPTH 5
# endif

/* Error codes */
typedef enum e_error
{
	ERROR_NONE = 0,
	ERROR_ARGS = 1,
	ERROR_FILE = 2,
	ERROR_PARSE = 3,
	ERROR_MEMORY = 4,
	ERROR_MLX = 5
}	t_error;

/* 3D Vector structure */
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

/* RGB Color structure */
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

/* Ray structure */
typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

/* Camera structure 
 * The move && rotate speed name are misleading
 * Those variables are used to define the movement
 * distance rather than speed of execution
 * */
typedef struct s_camera
{
	t_vec3	position;
	t_vec3	direction;
	t_vec3	up;
	t_vec3	right;
	double	fov;
	double	move_speed;
	double	rotate_speed;
}	t_camera;

/* Material structure (bonus) */
typedef struct s_material
{
	t_color	color;
	double	reflection;
	double	transparency;
	double	refraction_index;
}	t_material;

/* Light source structure */
typedef struct s_light
{
	t_vec3			position;
	double			intensity;
	t_color			color;
	struct s_light	*next;
}	t_light;

/* Geometric object structure */
typedef struct s_object
{
	int				uid;
	int				type;
	t_vec3			position;
	t_vec3			normal;
	t_vec3			axis;
	t_vec3			triangle[3];
	double			radius;
	double			height;
	double			angle;
	t_material		material;
	struct s_object	*next;
}	t_object;

/* Hit information structure */
typedef struct s_hit
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_material	*material;
	t_object	*object;
}	t_hit;

/* Axis-Aligned Bounding Box structure */
typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

/* Cylinder parameters structure */ 
typedef struct s_cylinder_params
{
	double	a;
	double	b;
	double	c;
}	t_cylinder_params;

/* Cone parameters structure */
typedef struct s_cone_params
{
	double	a;
	double	b;
	double	c;
}	t_cone_params;

/* Slab intersection result structure */
typedef struct s_slab_result
{
	double	t_min;
	double	t_max;
	int		valid;
}	t_slab_result;

/* Slab intersections structure */
typedef struct s_slab_intersections
{
	double	t1;
	double	t2;
}	t_slab_intersections;

/* BVH (Bounding Volume Hierarchy) node structure */
typedef struct s_bvh_node
{
    t_aabb              bounds;
    t_object            **objects;
    int                 object_count;
    struct s_bvh_node   *left;
    struct s_bvh_node   *right;
}	t_bvh_node;

/* Scene structure */
typedef struct s_scene
{
	t_camera	camera;
	t_light		*lights;
	t_object	*objects;
	t_color		ambient;
	double		ambient_ratio;
	t_bvh_node	*bvh_root;
}	t_scene;

/* BVH intersection query structure */
typedef struct s_aabb_query
{
	t_vec3	origin;
	t_vec3	dir;
	t_aabb	box;
	double	tmin;
	double	tmax;
}	t_aabb_query;

/* MLX context structure */
typedef struct s_mlx
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*img_data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
	int		width;
	int		height;
}	t_mlx;
/* Threads manipulation arguments */
typedef struct s_minirt t_minirt;
/* Directive draw threads structure */
typedef struct s_intels
{
	t_minirt *rt;
	int xstart;
	int ystart;
	int xend;
	int yend;
}	t_intels;

typedef struct s_threadArgs
{
	pthread_mutex_t mutexQueue;
	pthread_cond_t condQueue;
	int total_directives;
	int completed_directives;
	int stop;
	int ntask;
	t_intels directives_rendering[MAX_DIRS];
}	t_threadArgs;

typedef struct s_key_queue
{
	int	cap;
	int back;
	int front;
	int	key_queue[MAX_KEYS];
}	t_key_queue;

/* Main structure */
typedef struct s_minirt
{
	t_mlx			mlx;
	t_scene			scene;
	char			*filename;
	int				keys[KEY_ACTION_COUNT];
	t_threadArgs	args;
}	t_minirt;

//t_key_queue		key_queue;

/* ************************************************************************** */
/*                              MATH FUNCTIONS                               */
/* ************************************************************************** */

/* Vector operations */
t_vec3	vec3_new(double x, double y, double z);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_mult(t_vec3 v, double scalar);
t_vec3	vec3_div(t_vec3 v, double scalar);
double	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
t_vec3	vec3_normalize(t_vec3 v);
double	vec3_length(t_vec3 v);
double	vec3_length_squared(t_vec3 v);

/* Ray operations */
t_ray	ray_new(t_vec3 origin, t_vec3 direction);
t_vec3	ray_at(t_ray ray, double t);
t_ray	ray_transform(t_ray ray, t_vec3 translation);
t_ray	ray_reflect(t_ray incident, t_vec3 hit_point, t_vec3 normal);
double	ray_distance_to_point(t_ray ray, t_vec3 point);

/* ************************************************************************** */
/*                            PARSING FUNCTIONS                              */
/* ************************************************************************** */

int		validate_file_extension(char *filename);
int		validate_file_access(char *filename);
int		validate_arguments(int argc, char **argv);

int		parse_scene(char *filename, t_scene *scene);
int		parse_ambient(char *line, t_scene *scene);
int		parse_camera(char *line, t_scene *scene);
int		parse_light(char *line, t_scene *scene);
int		parse_sphere(char *line, t_scene *scene);
int		parse_plane(char *line, t_scene *scene);
int		parse_cylinder(char *line, t_scene *scene);
int		parse_cone(char *line, t_scene *scene);
int		parse_cube(char *line, t_scene *scene);
int		parse_triangle(char *line, t_scene *scene);

/* Parsing utility functions */
int		parse_double(char *str, double *result);
int		parse_int(char *str, int *result);
int		parse_vec3(char *str, t_vec3 *result);
int		parse_color(char *str, t_color *result);
char	*get_next_token(char **str);
void	add_object_to_scene(t_scene *scene, t_object *object);

/* ************************************************************************** */
/*                           INTERSECTION FUNCTIONS                          */
/* ************************************************************************** */

double	intersect_sphere(t_ray ray, t_object *sphere);
double	intersect_plane(t_ray ray, t_object *plane);
double	intersect_cylinder(t_ray ray, t_object *cylinder);
double	intersect_cone(t_ray ray, t_object *cone);
double	intersect_cube(t_ray ray, t_object *cube);
double	intersect_triangle(t_ray ray, t_object *triangle);
int		intersect_scene_linear(t_ray ray, t_scene *scene, t_hit *hit);

/* ************************************************************************** */
/*                            RENDERING FUNCTIONS                            */
/* ************************************************************************** */

t_color	raycast(t_ray ray, t_scene *scene, int depth);
t_color	raycast_optimized(t_ray ray, t_scene *scene, int depth);
t_color	calculate_lighting(t_vec3 point, t_vec3 normal, t_scene *scene,
			t_material *material);
int		is_in_shadow_with_dir(t_vec3 point, t_vec3 dir_to_light, double light_distance, t_scene *scene);
t_color	calculate_reflection(t_ray ray, t_hit *hit, t_scene *scene, int depth);
t_color	calculate_refraction(t_ray ray, t_hit *hit, t_scene *scene, int depth);
t_color	render_pixel_antialiased(t_minirt *rt, int x, int y);
t_color	render_pixel_adaptive(t_minirt *rt, int x, int y);
void	render_scene(t_minirt *rt);
void	render_scene_progressive(t_minirt *rt);
void	render_scene_optimized(t_minirt *rt);

/* Raytracer utility functions */
void	calculate_viewport_dimensions(t_minirt *rt, t_camera *camera,
			double *half_width, double *half_height);
t_vec3	calculate_pixel_world_position(t_camera *camera, double u,
			double v, double half_width, double half_height);
t_color	calculate_background_color(t_ray ray);
void	render_pixel_at_coordinates(t_minirt *rt, int x, int y,
			double inv_width, double inv_height);
t_color	calculate_hit_color(t_ray ray, t_hit *hit, t_scene *scene, int depth);

/* Progressive rendering functions */
void	fill_pixel_block(t_minirt *rt, int x, int y, int step,
			t_color pixel_color);
void	render_progressive_step(t_minirt *rt, int step,
			double inv_width, double inv_height);

/* ************************************************************************** */
/*                            GRAPHICS FUNCTIONS                             */
/* ************************************************************************** */

int		init_mlx(t_mlx *mlx);
void	put_pixel(t_mlx *mlx, int x, int y, t_color color);
int		color_to_int(t_color color);
t_color	int_to_color(int color);
void	display_image(t_mlx *mlx);
void	cleanup_mlx(t_mlx *mlx);

/* ************************************************************************** */
/*                             EVENT FUNCTIONS                               */
/* ************************************************************************** */

int		keycode_to_action(int keycode);
void	setup_hooks(t_minirt *rt);
void	zoom_camera(t_camera *cam, double factor);
int		handle_keypress(int keycode, t_minirt *rt);
int		handle_close(t_minirt *rt);
int		handle_loop(t_minirt *rt);
void	setup_hooks(t_minirt *rt);
void	capacity_threshold_handler(t_key_queue *queue);
void	enqueue(t_key_queue *queue, int key);
int		dequeue(t_key_queue *queue);

/* ************************************************************************** */
/*                             UTILITY FUNCTIONS                             */
/* ************************************************************************** */

void	error_exit(char *message, t_minirt *rt);
void	exit_success(t_minirt *rt);
void	print_error(char *message);
void	handle_malloc_error(size_t size, t_minirt *rt);
void	handle_file_error(char *filename, char *operation, t_minirt *rt);
void	handle_parse_error(char *filename, int line_num, char *message, t_minirt *rt);
void	cleanup_scene(t_scene *scene);
void	cleanup_all(t_minirt *rt);
int		validate_scene(t_scene *scene);
void	*safe_malloc(size_t size);
char	*safe_strdup(char *str);
void	safe_free(void **ptr);
void	*safe_calloc(size_t count, size_t size);
void	*safe_realloc(void *ptr, size_t size);
void	cleanup_string_array(char **array, int count);
void	cleanup_light_list(t_light *lights);
void	cleanup_object_list(t_object *objects);

/* ************************************************************************** */
/*                             COLOR FUNCTIONS                               */
/* ************************************************************************** */

t_color	color_new(int r, int g, int b);
t_color	color_add(t_color a, t_color b);
t_color	color_mult(t_color color, double scalar);
t_color	color_blend(t_color a, t_color b, double ratio);
t_color	color_clamp(t_color color);
t_color	color_gamma_correct(t_color color, double gamma);
t_color	color_linear_to_srgb(t_color color);

/* ************************************************************************** */
/*                            CAMERA FUNCTIONS                               */
/* ************************************************************************** */

void	setup_camera(t_camera *camera);
t_ray	get_camera_ray(t_minirt *rt, t_camera *camera, double u, double v);
void	move_camera(t_camera *camera, t_vec3 direction, double distance);
void	rotate_camera(t_camera *camera, double yaw, double pitch);
void	update_camera_vectors(t_camera *camera);

/* ************************************************************************** */
/*                              BVH FUNCTIONS                                */
/* ************************************************************************** */

t_bvh_node	*build_bvh(t_scene *scene);
t_bvh_node	*build_bvh_recursive(t_object **objects, int count);
t_aabb		get_object_bounds(t_object *object);
t_aabb		aabb_union(t_aabb a, t_aabb b);
void		setup_aabb_query(t_aabb_query *query, t_ray ray, t_aabb bounds);
double		aabb_surface(t_aabb a);
int	find_sah_split(t_object **objects, int count, int *best_axis, \
		int *best_index);
int			case_internal_node(t_bvh_node *node, t_hit *temp_hit, t_ray ray);	
int			case_leaf_node(t_bvh_node *node, t_hit *temp_hit, t_ray ray);
int			intersect_bvh_iter(t_ray ray, t_bvh_node *root, t_hit *hit);
int			intersect_aabb_query(t_aabb_query *query);
void		cleanup_bvh(t_bvh_node *node);
t_vec3		get_object_normal(t_vec3 hit_point, t_object *object);
void sort_objects_axis(t_object **objects, int count, int axis);

/* ************************************************************************** */
/*                             CUBE UTILITY FUNCTIONS                        */
/* ************************************************************************** */

void			get_cube_bounds(t_object *cube, t_vec3 *cube_min, t_vec3 *cube_max);
void			swap_values(double *t1, double *t2);
int				test_slab_intersection(double ray_dir, double ray_orig,
					double slab_min, double slab_max);
t_slab_intersections	calculate_slab_intersections(double ray_dir, double ray_orig,
					double slab_min, double slab_max);
t_slab_result	update_intersection_interval(t_slab_result prev,
					double t1, double t2);

/* ************************************************************************** */
/*                             CONE UTILITY FUNCTIONS                        */
/* ************************************************************************** */

void	get_cone_projections(t_ray ray, t_object *cone, t_vec3 *proj_ray,
			t_vec3 *proj_oc);

/* ************************************************************************** */
/*                            NORMAL FUNCTIONS                               */
/* ************************************************************************** */

t_vec3	get_sphere_normal(t_vec3 hit_point, t_object *sphere);
t_vec3	get_plane_normal(t_vec3 hit_point, t_object *plane);
t_vec3	get_cylinder_normal(t_vec3 hit_point, t_object *cylinder);
t_vec3	get_cone_normal(t_vec3 hit_point, t_object *cone);

#endif
