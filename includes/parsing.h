#ifndef PARSING_H
# define PARSING_H

void		parseLine(char *line);
bool		readContentAndCreateScene(t_scene *scene, int fd);
int			openFile(const char *file);
const char	*initParsing(int argc, char **argv, char **envp);

#endif
