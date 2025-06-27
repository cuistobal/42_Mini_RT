#include "minirt.h"

//Appends the new objet as the head or tail of the list.
static void	insertNewObject(t_object **head, t_object **tail, t_object *new)
{
	if (!*head)
	{
		*head = new;
		*tail = new;
	}
	else
	{
		(*tail)->next = new;
		*tail = (*tail)->next;
	}
}

//Ternaries and for loops rock
static bool insertSceneParameter(t_scene **scene, t_object *new, char type)
{
    if (type == EALIGHT)
    {
        if ((*scene)->ambiantLightning)
            return (false);
        (*scene)->ambiantLightning = new;
    }
    else if (type == ECAMERA)
    {
        if ((*scene)->camera)
            return (false);
        (*scene)->camera = new;
    }
    if (type == ELIGHT)
    {
        if ((*scene)->light)
            return (false);
        (*scene)->light = new;
    }
    return (false);
}

//This helper plces the new object where it belongs. If the object is a scene
//parameter, returns false if the parameter was already set (Subject imposes 
//that the source.rt file contains only 1 occurence of scene paraemeters).
bool    insertNewObjectInMinirt(t_minirt *minirt, t_object *new, t_object **tail)
{
    if (new->type > ELIGHT)
        return (insertNewObject(&minirt->scene->objects, tail, new), true);
    return (insertSceneParameter(&minirt->scene, new, new->type));
}
