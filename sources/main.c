#include "minirt.h"

void    printObject(t_object *obj)
{
    if (!obj)
        return ; 
    printf("type        ->  %c\n", obj->type);
    printf("colours     ->  ");
    for (int i = 0; i < 4; i++)
        printf("%ld \n", obj->colours[i]);
    printf("\n");
    printf("ratios      ->  ");
    for (int i = 0; i < 2; i++)
        printf("%f \n", obj->ratio[i]);
    printf("\n");
    printf("coordinates ->  "); 
    for (int i = 0; i < 3; i++)
        printf("%f \n", obj->coordinates[i]);
    printf("\n");
    printf("normVector ->  "); 
    for (int i = 0; i < 3; i++)
        printf("%f \n", obj->normVector[i]);
    printf("\n");
}

void    printMinirt(t_minirt *minirt)
{
    if (minirt->scene)
    {
        printObject(minirt->scene->light);
        printObject(minirt->scene->camera);
        printObject(minirt->scene->ambiantLightning);
        while (minirt->scene->objects)
        {
            printObject(minirt->scene->objects);
            minirt->scene->objects = minirt->scene->objects->next;
        }
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_minirt	minirt;

	if (!parsing(argc, argv, envp, &minirt))
		return (1);
    
	return (freeMinirt(&minirt), 0);
}
