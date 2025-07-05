#include "minirt.h"

//Appends the new objet as the head or tail of the list.
static void	insert_new_object(t_object **head, t_object **tail, t_object *new)
{
	if (!*head)
	{
		*head = new;
		*tail = new;
	}
	else
	{
		(*tail)->next = new;
		*tail = new;
	}
}

//Ternaries and for loops rock
static bool insert_scene_parameter(t_scene **scene, t_object *new, char type)
{
    if (type == EALIGHT && !(*scene)->ambiant)
        return ((*scene)->ambiant = new, true);
    else if (type == ECAMERA && !(*scene)->camera)
        return((*scene)->camera = new, true);
    if (type == ELIGHT && !(*scene)->light)
        return ((*scene)->light = new, true);
    return (false);
}

//This helper plces the new object where it belongs. If the object is a scene
//parameter, returns false if the parameter was already set (Subject imposes 
//that the source.rt file contains only 1 occurence of scene paraemeters).
bool    insert_new_object_in_minirt(t_minirt *minirt, t_object *new, t_object **tail)
{
    if (new->type > ELIGHT)
        return (insert_new_object(&minirt->scene->objects, tail, new), true);
    return (insert_scene_parameter(&minirt->scene, new, new->type));
}
