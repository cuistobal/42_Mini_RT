# Meshing in C for miniRT - Complete Tutorial

## Table of Contents
1. [Introduction to Meshing](#introduction)
2. [Basic Data Structures](#data-structures)
3. [Vertex and Face Management](#vertex-face)
4. [Mesh Loading from Files](#file-loading)
5. [Mesh Transformations](#transformations)
6. [Ray-Mesh Intersection](#intersection)
7. [Memory Management](#memory)
8. [Integration with miniRT](#integration)
9. [Advanced Techniques](#advanced)

## 1. Introduction to Meshing {#introduction}

Meshing in computer graphics refers to representing 3D objects as collections of polygons (usually triangles). In miniRT, you'll typically work with triangular meshes for complex geometric objects.

### Key Concepts:
- **Vertices**: 3D points in space
- **Faces/Triangles**: Groups of 3 vertices forming triangular surfaces
- **Normals**: Perpendicular vectors to surfaces for lighting calculations
- **UV Coordinates**: 2D texture mapping coordinates

## 2. Basic Data Structures {#data-structures}

```c
// Vector3 structure (you probably already have this)
typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;

// Vertex structure
typedef struct s_vertex
{
    t_vec3  position;
    t_vec3  normal;
    double  u, v;  // texture coordinates
} t_vertex;

// Triangle/Face structure
typedef struct s_triangle
{
    int     v1, v2, v3;  // indices to vertices
    t_vec3  normal;      // face normal
    int     material_id; // material index
} t_triangle;

// Mesh structure
typedef struct s_mesh
{
    t_vertex    *vertices;
    t_triangle  *triangles;
    int         vertex_count;
    int         triangle_count;
    t_vec3      center;
    double      radius;  // bounding sphere radius
    char        *name;
} t_mesh;
```

## 3. Vertex and Face Management {#vertex-face}

### Creating and Managing Vertices

```c
// Create a new vertex
t_vertex create_vertex(double x, double y, double z)
{
    t_vertex v;
    
    v.position.x = x;
    v.position.y = y;
    v.position.z = z;
    v.normal.x = 0;
    v.normal.y = 0;
    v.normal.z = 0;
    v.u = 0;
    v.v = 0;
    return (v);
}

// Calculate face normal from three vertices
t_vec3 calculate_face_normal(t_vertex v1, t_vertex v2, t_vertex v3)
{
    t_vec3 edge1, edge2, normal;
    
    // Calculate edges
    edge1.x = v2.position.x - v1.position.x;
    edge1.y = v2.position.y - v1.position.y;
    edge1.z = v2.position.z - v1.position.z;
    
    edge2.x = v3.position.x - v1.position.x;
    edge2.y = v3.position.y - v1.position.y;
    edge2.z = v3.position.z - v1.position.z;
    
    // Cross product
    normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
    normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
    normal.z = edge1.x * edge2.y - edge1.y * edge2.x;
    
    // Normalize
    double length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length > 0)
    {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }
    return (normal);
}

// Calculate vertex normals (smooth shading)
void calculate_vertex_normals(t_mesh *mesh)
{
    int i, j;
    t_vec3 face_normal;
    
    // Initialize all vertex normals to zero
    for (i = 0; i < mesh->vertex_count; i++)
    {
        mesh->vertices[i].normal.x = 0;
        mesh->vertices[i].normal.y = 0;
        mesh->vertices[i].normal.z = 0;
    }
    
    // Accumulate face normals to vertices
    for (i = 0; i < mesh->triangle_count; i++)
    {
        t_triangle *tri = &mesh->triangles[i];
        face_normal = calculate_face_normal(
            mesh->vertices[tri->v1],
            mesh->vertices[tri->v2],
            mesh->vertices[tri->v3]
        );
        
        // Add to each vertex of the triangle
        mesh->vertices[tri->v1].normal.x += face_normal.x;
        mesh->vertices[tri->v1].normal.y += face_normal.y;
        mesh->vertices[tri->v1].normal.z += face_normal.z;
        
        mesh->vertices[tri->v2].normal.x += face_normal.x;
        mesh->vertices[tri->v2].normal.y += face_normal.y;
        mesh->vertices[tri->v2].normal.z += face_normal.z;
        
        mesh->vertices[tri->v3].normal.x += face_normal.x;
        mesh->vertices[tri->v3].normal.y += face_normal.y;
        mesh->vertices[tri->v3].normal.z += face_normal.z;
    }
    
    // Normalize all vertex normals
    for (i = 0; i < mesh->vertex_count; i++)
    {
        t_vec3 *n = &mesh->vertices[i].normal;
        double length = sqrt(n->x * n->x + n->y * n->y + n->z * n->z);
        if (length > 0)
        {
            n->x /= length;
            n->y /= length;
            n->z /= length;
        }
    }
}
```

## 4. Mesh Loading from Files {#file-loading}

### Simple OBJ File Parser

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple OBJ loader (basic implementation)
t_mesh *load_obj_file(const char *filename)
{
    FILE *file;
    char line[256];
    t_mesh *mesh;
    int vertex_capacity = 100;
    int triangle_capacity = 100;
    
    file = fopen(filename, "r");
    if (!file)
        return (NULL);
    
    mesh = malloc(sizeof(t_mesh));
    mesh->vertices = malloc(sizeof(t_vertex) * vertex_capacity);
    mesh->triangles = malloc(sizeof(t_triangle) * triangle_capacity);
    mesh->vertex_count = 0;
    mesh->triangle_count = 0;
    
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            // Vertex line: v x y z
            double x, y, z;
            sscanf(line, "v %lf %lf %lf", &x, &y, &z);
            
            // Resize if needed
            if (mesh->vertex_count >= vertex_capacity)
            {
                vertex_capacity *= 2;
                mesh->vertices = realloc(mesh->vertices, 
                    sizeof(t_vertex) * vertex_capacity);
            }
            
            mesh->vertices[mesh->vertex_count] = create_vertex(x, y, z);
            mesh->vertex_count++;
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            // Face line: f v1 v2 v3 (1-indexed in OBJ)
            int v1, v2, v3;
            sscanf(line, "f %d %d %d", &v1, &v2, &v3);
            
            // Resize if needed
            if (mesh->triangle_count >= triangle_capacity)
            {
                triangle_capacity *= 2;
                mesh->triangles = realloc(mesh->triangles,
                    sizeof(t_triangle) * triangle_capacity);
            }
            
            // Convert to 0-indexed
            mesh->triangles[mesh->triangle_count].v1 = v1 - 1;
            mesh->triangles[mesh->triangle_count].v2 = v2 - 1;
            mesh->triangles[mesh->triangle_count].v3 = v3 - 1;
            mesh->triangle_count++;
        }
    }
    
    fclose(file);
    
    // Calculate normals
    calculate_vertex_normals(mesh);
    
    return (mesh);
}
```

## 5. Mesh Transformations {#transformations}

```c
// Transform mesh vertices
void transform_mesh(t_mesh *mesh, t_vec3 translation, t_vec3 rotation, t_vec3 scale)
{
    int i;
    
    for (i = 0; i < mesh->vertex_count; i++)
    {
        t_vec3 *pos = &mesh->vertices[i].position;
        
        // Scale
        pos->x *= scale.x;
        pos->y *= scale.y;
        pos->z *= scale.z;
        
        // Rotate (simplified - you might want to use matrices)
        rotate_point(pos, rotation);
        
        // Translate
        pos->x += translation.x;
        pos->y += translation.y;
        pos->z += translation.z;
    }
    
    // Recalculate normals after transformation
    calculate_vertex_normals(mesh);
}

// Simple rotation function (you can expand this)
void rotate_point(t_vec3 *point, t_vec3 rotation)
{
    // Rotate around Y axis
    if (rotation.y != 0)
    {
        double cos_y = cos(rotation.y);
        double sin_y = sin(rotation.y);
        double x = point->x * cos_y - point->z * sin_y;
        double z = point->x * sin_y + point->z * cos_y;
        point->x = x;
        point->z = z;
    }
    
    // Add X and Z rotations as needed
}
```

## 6. Ray-Mesh Intersection {#intersection}

```c
// Ray-triangle intersection using Möller-Trumbore algorithm
int ray_triangle_intersect(t_vec3 ray_origin, t_vec3 ray_direction,
                          t_vertex v0, t_vertex v1, t_vertex v2,
                          double *t, double *u, double *v)
{
    t_vec3 edge1, edge2, h, s, q;
    double a, f;
    
    // Calculate edges
    edge1.x = v1.position.x - v0.position.x;
    edge1.y = v1.position.y - v0.position.y;
    edge1.z = v1.position.z - v0.position.z;
    
    edge2.x = v2.position.x - v0.position.x;
    edge2.y = v2.position.y - v0.position.y;
    edge2.z = v2.position.z - v0.position.z;
    
    // Cross product: ray_direction x edge2
    h.x = ray_direction.y * edge2.z - ray_direction.z * edge2.y;
    h.y = ray_direction.z * edge2.x - ray_direction.x * edge2.z;
    h.z = ray_direction.x * edge2.y - ray_direction.y * edge2.x;
    
    // Dot product: edge1 . h
    a = edge1.x * h.x + edge1.y * h.y + edge1.z * h.z;
    
    if (a > -0.00001 && a < 0.00001)
        return (0); // Ray is parallel to triangle
    
    f = 1.0 / a;
    
    s.x = ray_origin.x - v0.position.x;
    s.y = ray_origin.y - v0.position.y;
    s.z = ray_origin.z - v0.position.z;
    
    *u = f * (s.x * h.x + s.y * h.y + s.z * h.z);
    
    if (*u < 0.0 || *u > 1.0)
        return (0);
    
    // Cross product: s x edge1
    q.x = s.y * edge1.z - s.z * edge1.y;
    q.y = s.z * edge1.x - s.x * edge1.z;
    q.z = s.x * edge1.y - s.y * edge1.x;
    
    *v = f * (ray_direction.x * q.x + ray_direction.y * q.y + ray_direction.z * q.z);
    
    if (*v < 0.0 || *u + *v > 1.0)
        return (0);
    
    *t = f * (edge2.x * q.x + edge2.y * q.y + edge2.z * q.z);
    
    return (*t > 0.00001); // Intersection found
}

// Check ray intersection with entire mesh
int ray_mesh_intersect(t_mesh *mesh, t_vec3 ray_origin, t_vec3 ray_direction,
                       double *closest_t, int *triangle_index)
{
    int i;
    int hit = 0;
    double t, u, v;
    *closest_t = INFINITY;
    
    for (i = 0; i < mesh->triangle_count; i++)
    {
        t_triangle *tri = &mesh->triangles[i];
        
        if (ray_triangle_intersect(ray_origin, ray_direction,
                                  mesh->vertices[tri->v1],
                                  mesh->vertices[tri->v2],
                                  mesh->vertices[tri->v3],
                                  &t, &u, &v))
        {
            if (t < *closest_t)
            {
                *closest_t = t;
                *triangle_index = i;
                hit = 1;
            }
        }
    }
    
    return (hit);
}
```

## 7. Memory Management {#memory}

```c
// Free mesh memory
void free_mesh(t_mesh *mesh)
{
    if (mesh)
    {
        if (mesh->vertices)
            free(mesh->vertices);
        if (mesh->triangles)
            free(mesh->triangles);
        if (mesh->name)
            free(mesh->name);
        free(mesh);
    }
}

// Create empty mesh
t_mesh *create_mesh(int vertex_count, int triangle_count)
{
    t_mesh *mesh = malloc(sizeof(t_mesh));
    
    mesh->vertices = malloc(sizeof(t_vertex) * vertex_count);
    mesh->triangles = malloc(sizeof(t_triangle) * triangle_count);
    mesh->vertex_count = vertex_count;
    mesh->triangle_count = triangle_count;
    mesh->name = NULL;
    
    return (mesh);
}
```

## 8. Integration with miniRT {#integration}

```c
// Add mesh to your scene structure
typedef struct s_scene
{
    // ... your existing scene members
    t_mesh  **meshes;
    int     mesh_count;
} t_scene;

// Modified intersection function for your ray tracer
int intersect_scene(t_scene *scene, t_vec3 ray_origin, t_vec3 ray_direction,
                   double *closest_t, int *object_type, int *object_index)
{
    int hit = 0;
    double t;
    int triangle_index;
    
    *closest_t = INFINITY;
    
    // Check spheres, planes, etc. (your existing code)
    // ...
    
    // Check meshes
    for (int i = 0; i < scene->mesh_count; i++)
    {
        if (ray_mesh_intersect(scene->meshes[i], ray_origin, ray_direction, &t, &triangle_index))
        {
            if (t < *closest_t)
            {
                *closest_t = t;
                *object_type = MESH_TYPE; // Define this constant
                *object_index = i;
                hit = 1;
            }
        }
    }
    
    return (hit);
}
```

## 9. Advanced Techniques {#advanced}

### Bounding Volume Hierarchy (BVH) for Performance

```c
// Bounding box structure
typedef struct s_bbox
{
    t_vec3 min;
    t_vec3 max;
} t_bbox;

// Calculate bounding box for mesh
t_bbox calculate_mesh_bbox(t_mesh *mesh)
{
    t_bbox box;
    
    if (mesh->vertex_count == 0)
        return (box);
    
    box.min = mesh->vertices[0].position;
    box.max = mesh->vertices[0].position;
    
    for (int i = 1; i < mesh->vertex_count; i++)
    {
        t_vec3 pos = mesh->vertices[i].position;
        
        if (pos.x < box.min.x) box.min.x = pos.x;
        if (pos.y < box.min.y) box.min.y = pos.y;
        if (pos.z < box.min.z) box.min.z = pos.z;
        
        if (pos.x > box.max.x) box.max.x = pos.x;
        if (pos.y > box.max.y) box.max.y = pos.y;
        if (pos.z > box.max.z) box.max.z = pos.z;
    }
    
    return (box);
}

// Ray-box intersection test
int ray_box_intersect(t_vec3 ray_origin, t_vec3 ray_direction, t_bbox box)
{
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    
    tmin = (box.min.x - ray_origin.x) / ray_direction.x;
    tmax = (box.max.x - ray_origin.x) / ray_direction.x;
    
    if (tmin > tmax)
    {
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    
    tymin = (box.min.y - ray_origin.y) / ray_direction.y;
    tymax = (box.max.y - ray_origin.y) / ray_direction.y;
    
    if (tymin > tymax)
    {
        double temp = tymin;
        tymin = tymax;
        tymax = temp;
    }
    
    if (tmin > tymax || tymin > tmax)
        return (0);
    
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    
    tzmin = (box.min.z - ray_origin.z) / ray_direction.z;
    tzmax = (box.max.z - ray_origin.z) / ray_direction.z;
    
    if (tzmin > tzmax)
    {
        double temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }
    
    if (tmin > tzmax || tzmin > tmax)
        return (0);
    
    return (1);
}
```

### Creating Simple Procedural Meshes

```c
// Create a cube mesh
t_mesh *create_cube_mesh(double size)
{
    t_mesh *mesh = create_mesh(8, 12); // 8 vertices, 12 triangles
    double half = size / 2.0;
    
    // Define cube vertices
    mesh->vertices[0] = create_vertex(-half, -half, -half);
    mesh->vertices[1] = create_vertex( half, -half, -half);
    mesh->vertices[2] = create_vertex( half,  half, -half);
    mesh->vertices[3] = create_vertex(-half,  half, -half);
    mesh->vertices[4] = create_vertex(-half, -half,  half);
    mesh->vertices[5] = create_vertex( half, -half,  half);
    mesh->vertices[6] = create_vertex( half,  half,  half);
    mesh->vertices[7] = create_vertex(-half,  half,  half);
    
    // Define triangles (two per face)
    int faces[12][3] = {
        {0, 1, 2}, {0, 2, 3}, // front
        {4, 7, 6}, {4, 6, 5}, // back
        {0, 4, 5}, {0, 5, 1}, // bottom
        {2, 6, 7}, {2, 7, 3}, // top
        {0, 3, 7}, {0, 7, 4}, // left
        {1, 5, 6}, {1, 6, 2}  // right
    };
    
    for (int i = 0; i < 12; i++)
    {
        mesh->triangles[i].v1 = faces[i][0];
        mesh->triangles[i].v2 = faces[i][1];
        mesh->triangles[i].v3 = faces[i][2];
    }
    
    calculate_vertex_normals(mesh);
    return (mesh);
}
```

## Tips for miniRT Implementation

1. **Start Simple**: Begin with basic triangle rendering before moving to complex meshes
2. **Memory Management**: Always free allocated memory and check for allocation failures
3. **Performance**: Use bounding boxes for early rejection of rays that don't hit the mesh
4. **Normals**: Proper normal calculation is crucial for realistic lighting
5. **File Formats**: Start with simple OBJ files before supporting more complex formats
6. **Debugging**: Implement mesh visualization/debugging functions to help troubleshoot

## Common Issues and Solutions

- **Inside-out triangles**: Make sure your triangle winding order is consistent
- **Z-fighting**: Use proper epsilon values in intersection tests
- **Memory leaks**: Always pair malloc with free
- **Performance**: Consider spatial data structures for large meshes
- **Precision**: Use appropriate floating-point precision for your calculations

This tutorial provides a solid foundation for implementing mesh support in your miniRT project. Start with the basic structures and gradually add complexity as needed.
