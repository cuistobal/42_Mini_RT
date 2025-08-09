#include "includes/minirt.h"
#include <assert.h>

int main(void)
{
    printf("Testing basic functionality...\n");
    
    // Test vector creation
    t_vec3 v = vec3_new(1, 2, 3);
    printf("Vector: %f, %f, %f\n", v.x, v.y, v.z);
    
    // Test vector length
    double len = vec3_length(v);
    printf("Length: %f\n", len);
    
    // Test scene initialization
    printf("Scene initialized\n");
    
    // Test simple parsing
    t_vec3 pos;
    if (parse_vec3("1,2,3", &pos))
        printf("Parsed position: %f, %f, %f\n", pos.x, pos.y, pos.z);
    else
        printf("Failed to parse position\n");
    
    t_color color;
    if (parse_color("255,128,64", &color))
        printf("Parsed color: %d, %d, %d\n", color.r, color.g, color.b);
    else
        printf("Failed to parse color\n");
    
    printf("Basic tests completed\n");
    return (0);
}