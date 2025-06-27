#ifndef PARSING_H
# define PARSING_H

bool    insertNewObjectInMinirt(t_minirt *minirt, t_object *new, t_object **tail);
bool    fillObject(t_object **object, char *str);
bool    readFileContentAndCreateScene(t_minirt *minirt, int fd);
bool    parsing(int argc, char **argv, char **envp, t_minirt *minirt);

//sources/parsing/fillObjectHelpers.c
bool	convertToFloatLimitless(float *coordinates, char *str);
bool	convertToFloatEnforceLimits(float *ratio, char *str, float min, float max);

#endif
