#include "includes/minirt.h"
#include <assert.h>

int main(void)
{
    printf("Testing cylinder parsing...\n");
    
    t_scene scene = {0};
    char cylinder_line[] = "50.0,0.0,20.6 0,0,1 14.2 21.42 10,0,255";
    
    printf("Testing cylinder parsing...\n");
    int result = parse_cylinder(cylinder_line, &scene);
    printf("Parse result: %d\n", result);
    
    if (scene.objects)
    {
        printf("Object created successfully\n");
        printf("Type: %d\n", scene.objects->type);
        printf("Position: %f, %f, %f\n", scene.objects->position.x, scene.objects->position.y, scene.objects->position.z);
        printf("Axis: %f, %f, %f\n", scene.objects->axis.x, scene.objects->axis.y, scene.objects->axis.z);
        printf("Radius: %f\n", scene.objects->radius);
        printf("Height: %f\n", scene.objects->height);
        printf("Color: %d, %d, %d\n", scene.objects->material.color.r, scene.objects->material.color.g, scene.objects->material.color.b);
    }
    else
    {
        printf("No object created\n");
    }
    
    return (0);
}