#include "includes/minirt.h"
#include <assert.h>

int main(void)
{
    printf("Testing object parsing...\n");
    
    t_scene scene = {0};
    char sphere_line[] = "0,0,20 10 255,0,0";
    
    printf("Testing sphere parsing...\n");
    int result = parse_sphere(sphere_line, &scene);
    printf("Parse result: %d\n", result);
    
    if (scene.objects)
    {
        printf("Object created successfully\n");
        printf("Type: %d\n", scene.objects->type);
        printf("Position: %f, %f, %f\n", scene.objects->position.x, scene.objects->position.y, scene.objects->position.z);
        printf("Radius: %f\n", scene.objects->radius);
        printf("Color: %d, %d, %d\n", scene.objects->material.color.r, scene.objects->material.color.g, scene.objects->material.color.b);
    }
    else
    {
        printf("No object created\n");
    }
    
    return (0);
}