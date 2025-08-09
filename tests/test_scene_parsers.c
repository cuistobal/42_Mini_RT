/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scene_parsers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include <assert.h>

/*
** count_objects - Count objects in scene
*/
int count_objects(t_scene *scene)
{
    int count = 0;
    t_object *current = scene->objects;
    
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

/*
** Test complete scene parsing
*/
void test_complete_scene_parsing(void)
{
    t_scene scene = {0};
    
    printf("Testing complete scene parsing...\n");
    
    // Parse the test scene file
    int result = parse_scene("tests/test_scene.rt", &scene);
    printf("Parse result: %d\n", result);
    
    if (result)
    {
        // Check that we have 3 objects
        int object_count = count_objects(&scene);
        printf("Object count: %d\n", object_count);
        assert(object_count == 3);
        
        // Check object types in order
        t_object *current = scene.objects;
        assert(current->type == SPHERE);
        printf("First object: SPHERE\n");
        
        current = current->next;
        assert(current->type == PLANE);
        printf("Second object: PLANE\n");
        
        current = current->next;
        assert(current->type == CYLINDER);
        printf("Third object: CYLINDER\n");
        
        assert(current->next == NULL);
        
        printf("Complete scene parsing test passed!\n");
    }
    else
    {
        printf("Failed to parse scene\n");
        assert(0);
    }
}

/*
** Main test function
*/
int main(void)
{
    printf("Running scene parsing tests...\n\n");
    
    test_complete_scene_parsing();
    
    printf("\nAll scene parsing tests passed successfully!\n");
    return (0);
}