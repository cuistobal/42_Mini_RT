#ifndef PARSING_H
# define PARSING_H

t_minirt	*readContentAndCreateScene(t_scene *scene, int fd);
t_minirt	*parsing(int argc, char **argv, char **envp);

#endif
