/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scene_parsers_simple.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include <assert.h>

// Stub functions for missing parsers
int parse_cone(char *line, t_scene *scene) { (void)line; (void)scene; return 0; }
int parse_cube(char *line, t_scene *scene) { (void)line; (void)scene; return 0; }
void cleanup_mlx(t_mlx *mlx) { (void)mlx; }

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
    
    // Initialize scene with default values for validation
    scene.camera.fov = -1; // Will be set by parser
    scene.ambient_ratio = -1; // Will be set by parser
    
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
        
        // Check camera was parsed
        assert(scene.camera.fov > 0);
        printf("Camera FOV: %f\n", scene.camera.fov);
        
        // Check ambient lighting was parsed
        assert(scene.ambient_ratio >= 0);
        printf("Ambient ratio: %f\n", scene.ambient_ratio);
        
        // Check lights were parsed
        assert(scene.lights != NULL);
        printf("Lights parsed successfully\n");
        
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