#ifndef TEMP_H
# define TEMP_H

# include <assert.h>
# include <string.h>
# include <ctype.h>

void	print_vec(const char *label, t_vec vector);
void	print_tree_bfs(const t_bvh *root);
void    print_object(const t_object *obj);
void    print_minirt(const t_minirt minirt);

#endif
