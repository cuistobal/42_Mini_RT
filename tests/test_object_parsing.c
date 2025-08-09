/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_object_parsing.c                              :+:      :+:    :+:   */
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
** Test sphere parsing
*/
void test_parse_sphere(void)
{
    t_scene scene = {0};
    char *line = "0,0,20 10 255,0,0";
    
    printf("Testing sphere parsing...\n");
    
    // Test valid sphere
    assert(parse_sphere(line, &scene) == 1);
    assert(scene.objects != NULL);
    assert(scene.objects->type == SPHERE);
    assert(scene.objects->position.x == 0);
    assert(scene.objects->position.y == 0);
    assert(scene.objects->position.z == 20);
    assert(scene.objects->radius == 5); // diameter/2
    assert(scene.objects->material.color.r == 255);
    assert(scene.objects->material.color.g == 0);
    assert(scene.objects->material.color.b == 0);
    
    // Test invalid sphere (negative diameter)
    t_scene scene2 = {0};
    char *invalid_line = "0,0,20 -10 255,0,0";
    assert(parse_sphere(invalid_line, &scene2) == 0);
    
    printf("Sphere parsing tests passed!\n");
}

/*
** Test plane parsing
*/
void test_parse_plane(void)
{
    t_scene scene = {0};
    char *line = "0,0,0 0,1,0 100,100,100";
    
    printf("Testing plane parsing...\n");
    
    // Test valid plane
    assert(parse_plane(line, &scene) == 1);
    assert(scene.objects != NULL);
    assert(scene.objects->type == PLANE);
    assert(scene.objects->position.x == 0);
    assert(scene.objects->position.y == 0);
    assert(scene.objects->position.z == 0);
    assert(scene.objects->normal.x == 0);
    assert(scene.objects->normal.y == 1);
    assert(scene.objects->normal.z == 0);
    assert(scene.objects->material.color.r == 100);
    assert(scene.objects->material.color.g == 100);
    assert(scene.objects->material.color.b == 100);
    
    // Test invalid plane (zero normal vector)
    t_scene scene2 = {0};
    char *invalid_line = "0,0,0 0,0,0 100,100,100";
    assert(parse_plane(invalid_line, &scene2) == 0);
    
    printf("Plane parsing tests passed!\n");
}

/*
** Test cylinder parsing
*/
void test_parse_cylinder(void)
{
    t_scene scene = {0};
    char *line = "50.0,0.0,20.6 0,0,1 14.2 21.42 10,0,255";
    
    printf("Testing cylinder parsing...\n");
    
    // Test valid cylinder
    assert(parse_cylinder(line, &scene) == 1);
    assert(scene.objects != NULL);
    assert(scene.objects->type == CYLINDER);
    assert(scene.objects->position.x == 50.0);
    assert(scene.objects->position.y == 0.0);
    assert(scene.objects->position.z == 20.6);
    assert(scene.objects->axis.x == 0);
    assert(scene.objects->axis.y == 0);
    assert(scene.objects->axis.z == 1);
    assert(scene.objects->radius == 7.1); // diameter/2
    assert(scene.objects->height == 21.42);
    assert(scene.objects->material.color.r == 10);
    assert(scene.objects->material.color.g == 0);
    assert(scene.objects->material.color.b == 255);
    
    // Test invalid cylinder (negative diameter)
    t_scene scene2 = {0};
    char *invalid_line = "0,0,0 0,1,0 -5 10 255,0,0";
    assert(parse_cylinder(invalid_line, &scene2) == 0);
    
    // Test invalid cylinder (negative height)
    t_scene scene3 = {0};
    char *invalid_line2 = "0,0,0 0,1,0 5 -10 255,0,0";
    assert(parse_cylinder(invalid_line2, &scene3) == 0);
    
    // Test invalid cylinder (zero axis)
    t_scene scene4 = {0};
    char *invalid_line3 = "0,0,0 0,0,0 5 10 255,0,0";
    assert(parse_cylinder(invalid_line3, &scene4) == 0);
    
    printf("Cylinder parsing tests passed!\n");
}/*

** Test multiple objects in scene (linked list management)
*/
void test_multiple_objects(void)
{
    t_scene scene = {0};
    char *sphere_line = "0,0,20 10 255,0,0";
    char *plane_line = "0,0,0 0,1,0 100,100,100";
    char *cylinder_line = "50.0,0.0,20.6 0,0,1 14.2 21.42 10,0,255";
    
    printf("Testing multiple objects parsing...\n");
    
    // Add sphere
    assert(parse_sphere(sphere_line, &scene) == 1);
    assert(scene.objects != NULL);
    assert(scene.objects->type == SPHERE);
    assert(scene.objects->next == NULL);
    
    // Add plane
    assert(parse_plane(plane_line, &scene) == 1);
    assert(scene.objects->next != NULL);
    assert(scene.objects->next->type == PLANE);
    assert(scene.objects->next->next == NULL);
    
    // Add cylinder
    assert(parse_cylinder(cylinder_line, &scene) == 1);
    assert(scene.objects->next->next != NULL);
    assert(scene.objects->next->next->type == CYLINDER);
    assert(scene.objects->next->next->next == NULL);
    
    // Verify order: sphere -> plane -> cylinder
    t_object *current = scene.objects;
    assert(current->type == SPHERE);
    current = current->next;
    assert(current->type == PLANE);
    current = current->next;
    assert(current->type == CYLINDER);
    assert(current->next == NULL);
    
    printf("Multiple objects parsing tests passed!\n");
}

/*
** Test object validation
*/
void test_object_validation(void)
{
    t_scene scene = {0};
    
    printf("Testing object validation...\n");
    
    // Test invalid parameters
    assert(parse_sphere(NULL, &scene) == 0);
    assert(parse_sphere("invalid", &scene) == 0);
    assert(parse_sphere("0,0,0", &scene) == 0); // Missing parameters
    
    assert(parse_plane(NULL, &scene) == 0);
    assert(parse_plane("invalid", &scene) == 0);
    assert(parse_plane("0,0,0", &scene) == 0); // Missing parameters
    
    assert(parse_cylinder(NULL, &scene) == 0);
    assert(parse_cylinder("invalid", &scene) == 0);
    assert(parse_cylinder("0,0,0", &scene) == 0); // Missing parameters
    
    printf("Object validation tests passed!\n");
}

/*
** Main test function
*/
int main(void)
{
    printf("Running object parsing tests...\n\n");
    
    test_parse_sphere();
    test_parse_plane();
    test_parse_cylinder();
    test_multiple_objects();
    test_object_validation();
    
    printf("\nAll object parsing tests passed successfully!\n");
    return (0);
}