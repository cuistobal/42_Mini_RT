#include "minirt.h"

//
void    printObject(t_object *obj)
{
    if (!obj)
        return ; 
    printf("type        	-> 	%c\n", obj->type);
    printf("colours     	->	");
    for (int i = 0; i < 4; i++)
        printf("%ld	", obj->colours[i]);
    printf("\n");
    printf("ratios      	->	");
    for (int i = 0; i < 2; i++)
        printf("%f	", obj->ratio[i]);
    printf("\n");
    printf("coordinates	->	"); 
    for (int i = 0; i < 3; i++)
        printf("%f	", obj->coordinates[i]);
    printf("\n");
    printf("normVector	->	"); 
    for (int i = 0; i < 3; i++)
        printf("%f	", obj->normVector[i]);
    printf("\n\n");
}

//
void    printMinirt(t_minirt minirt)
{
	t_object	*current;

    if (minirt.scene)
    {
        printObject(minirt.scene->light);
        printObject(minirt.scene->camera);
        printObject(minirt.scene->ambiantLightning);
		current = minirt.scene->objects;
        while (current)
        {
            printObject(current);
			current = current->next;
        }
    }
}

//
void	printTreeBFS(t_bvh *root)
{
	if (!root)
		return ;
	printTreeBFS(root->left);
	printTreeBFS(root->right);
	printObject(root->objects);
}
