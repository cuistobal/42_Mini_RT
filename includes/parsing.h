#ifndef PARSING_H
# define PARSING_H

bool    readFileContentAndCreateScene(t_minirt *minirt, int fd);
bool    parsing(int argc, char **argv, char **envp, t_minirt *minirt);

#endif
