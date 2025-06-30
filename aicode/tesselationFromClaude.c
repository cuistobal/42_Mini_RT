// Converting Geometric Primitives to Meshes
#include <math.h>
#include <stdlib.h>

// Reuse the mesh structures from the previous tutorial
typedef struct s_vec3 {
    double x, y, z;
} t_vec3;

typedef struct s_vertex {
    t_vec3 position;
    t_vec3 normal;
    double u, v;  // texture coordinates
} t_vertex;

typedef struct s_triangle {
    int v1, v2, v3;  // indices to vertices
    t_vec3 normal;
    int material_id;
} t_triangle;

typedef struct s_mesh {
    t_vertex *vertices;
    t_triangle *triangles;
    int vertex_count;
    int triangle_count;
    t_vec3 center;
    double radius;
    char *name;
} t_mesh;

// Your parsed object structures
typedef struct s_sphere_data {
    t_vec3 center;
    double diameter;
    int color[3];  // RGB
} t_sphere_data;

typedef struct s_plane_data {
    t_vec3 point;
    t_vec3 normal;
    int color[3];
} t_plane_data;

typedef struct s_cylinder_data {
    t_vec3 center;
    t_vec3 axis;
    double diameter;
    double height;
    int color[3];
} t_cylinder_data;

// Vector utilities
t_vec3 vec3_add(t_vec3 a, t_vec3 b) {
    return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

t_vec3 vec3_scale(t_vec3 v, double s) {
    return (t_vec3){v.x * s, v.y * s, v.z * s};
}

t_vec3 vec3_normalize(t_vec3 v) {
    double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len < 1e-9) return v;
    return (t_vec3){v.x / len, v.y / len, v.z / len};
}

t_vec3 vec3_cross(t_vec3 a, t_vec3 b) {
    return (t_vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// Create vertex with position and normal
t_vertex create_vertex_with_normal(t_vec3 pos, t_vec3 normal, double u, double v) {
    t_vertex vertex;
    vertex.position = pos;
    vertex.normal = vec3_normalize(normal);
    vertex.u = u;
    vertex.v = v;
    return vertex;
}

// ============================================================================
// SPHERE TO MESH CONVERSION
// ============================================================================

// Convert sphere to UV sphere mesh
t_mesh *sphere_to_mesh(t_sphere_data sphere_data, int u_segments, int v_segments) {
    int vertex_count = (u_segments + 1) * (v_segments + 1);
    int triangle_count = u_segments * v_segments * 2;
    
    t_mesh *mesh = malloc(sizeof(t_mesh));
    mesh->vertices = malloc(sizeof(t_vertex) * vertex_count);
    mesh->triangles = malloc(sizeof(t_triangle) * triangle_count);
    mesh->vertex_count = vertex_count;
    mesh->triangle_count = triangle_count;
    
    double radius = sphere_data.diameter / 2.0;
    int vertex_index = 0;
    
    // Generate vertices
    for (int v = 0; v <= v_segments; v++) {
        for (int u = 0; u <= u_segments; u++) {
            double theta = (double)u / u_segments * 2.0 * M_PI;  // Longitude
            double phi = (double)v / v_segments * M_PI;          // Latitude
            
            // Spherical coordinates to cartesian
            double x = radius * sin(phi) * cos(theta);
            double y = radius * cos(phi);
            double z = radius * sin(phi) * sin(theta);
            
            t_vec3 position = vec3_add(sphere_data.center, (t_vec3){x, y, z});
            t_vec3 normal = vec3_normalize((t_vec3){x, y, z}); // Normal points outward
            
            double tex_u = (double)u / u_segments;
            double tex_v = (double)v / v_segments;
            
            mesh->vertices[vertex_index++] = create_vertex_with_normal(position, normal, tex_u, tex_v);
        }
    }
    
    // Generate triangles
    int triangle_index = 0;
    for (int v = 0; v < v_segments; v++) {
        for (int u = 0; u < u_segments; u++) {
            int current = v * (u_segments + 1) + u;
            int next = current + u_segments + 1;
            
            // First triangle
            mesh->triangles[triangle_index].v1 = current;
            mesh->triangles[triangle_index].v2 = next;
            mesh->triangles[triangle_index].v3 = current + 1;
            triangle_index++;
            
            // Second triangle
            mesh->triangles[triangle_index].v1 = current + 1;
            mesh->triangles[triangle_index].v2 = next;
            mesh->triangles[triangle_index].v3 = next + 1;
            triangle_index++;
        }
    }
    
    mesh->center = sphere_data.center;
    mesh->radius = radius;
    mesh->name = strdup("sphere_mesh");
    
    return mesh;
}

// ============================================================================
// PLANE TO MESH CONVERSION
// ============================================================================

// Convert infinite plane to finite mesh (create a large quad)
t_mesh *plane_to_mesh(t_plane_data plane_data, double size) {
    t_mesh *mesh = malloc(sizeof(t_mesh));
    mesh->vertices = malloc(sizeof(t_vertex) * 4);
    mesh->triangles = malloc(sizeof(t_triangle) * 2);
    mesh->vertex_count = 4;
    mesh->triangle_count = 2;
    
    // Create two perpendicular vectors to the normal
    t_vec3 up = (fabs(plane_data.normal.y) < 0.9) ? (t_vec3){0, 1, 0} : (t_vec3){1, 0, 0};
    t_vec3 right = vec3_normalize(vec3_cross(plane_data.normal, up));
    up = vec3_normalize(vec3_cross(right, plane_data.normal));
    
    // Scale vectors to create plane of desired size
    right = vec3_scale(right, size);
    up = vec3_scale(up, size);
    
    // Create four corners of the plane
    t_vec3 corners[4] = {
        vec3_add(vec3_add(plane_data.point, right), up),      // +right +up
        vec3_add(vec3_add(plane_data.point, vec3_scale(right, -1)), up),  // -right +up
        vec3_add(vec3_add(plane_data.point, vec3_scale(right, -1)), vec3_scale(up, -1)), // -right -up
        vec3_add(vec3_add(plane_data.point, right), vec3_scale(up, -1))   // +right -up
    };
    
    // Create vertices
    double uvs[4][2] = {{1, 1}, {0, 1}, {0, 0}, {1, 0}};
    for (int i = 0; i < 4; i++) {
        mesh->vertices[i] = create_vertex_with_normal(corners[i], plane_data.normal, uvs[i][0], uvs[i][1]);
    }
    
    // Create triangles (two triangles forming a quad)
    mesh->triangles[0] = (t_triangle){0, 1, 2, plane_data.normal, 0};
    mesh->triangles[1] = (t_triangle){0, 2, 3, plane_data.normal, 0};
    
    mesh->center = plane_data.point;
    mesh->radius = size * 1.414; // Diagonal of square
    mesh->name = strdup("plane_mesh");
    
    return mesh;
}

// ============================================================================
// CYLINDER TO MESH CONVERSION
// ============================================================================

// Convert cylinder to mesh
t_mesh *cylinder_to_mesh(t_cylinder_data cylinder_data, int radial_segments) {
    // Vertices: radial_segments * 2 (top + bottom circles) + 2 (center points for caps)
    int circle_vertices = radial_segments * 2;
    int cap_vertices = 2; // Center points for top and bottom caps
    int vertex_count = circle_vertices + cap_vertices;
    
    // Triangles: radial_segments * 2 (sides) + radial_segments * 2 (caps)
    int triangle_count = radial_segments * 4;
    
    t_mesh *mesh = malloc(sizeof(t_mesh));
    mesh->vertices = malloc(sizeof(t_vertex) * vertex_count);
    mesh->triangles = malloc(sizeof(t_triangle) * triangle_count);
    mesh->vertex_count = vertex_count;
    mesh->triangle_count = triangle_count;
    
    double radius = cylinder_data.diameter / 2.0;
    double half_height = cylinder_data.height / 2.0;
    
    // Normalize the axis
    t_vec3 axis = vec3_normalize(cylinder_data.axis);
    
    // Create perpendicular vectors for the circular cross-section
    t_vec3 up = (fabs(axis.y) < 0.9) ? (t_vec3){0, 1, 0} : (t_vec3){1, 0, 0};
    t_vec3 u_vec = vec3_normalize(vec3_cross(axis, up));
    t_vec3 v_vec = vec3_normalize(vec3_cross(axis, u_vec));
    
    int vertex_index = 0;
    
    // Generate vertices for top and bottom circles
    for (int circle = 0; circle < 2; circle++) {
        double y_offset = (circle == 0) ? -half_height : half_height;
        t_vec3 circle_center = vec3_add(cylinder_data.center, vec3_scale(axis, y_offset));
        
        for (int i = 0; i < radial_segments; i++) {
            double angle = 2.0 * M_PI * i / radial_segments;
            double cos_a = cos(angle);
            double sin_a = sin(angle);
            
            // Position on circle
            t_vec3 circle_pos = vec3_add(
                vec3_add(circle_center, vec3_scale(u_vec, radius * cos_a)),
                vec3_scale(v_vec, radius * sin_a)
            );
            
            // Normal for side faces (perpendicular to axis, pointing outward)
            t_vec3 side_normal = vec3_normalize(vec3_add(
                vec3_scale(u_vec, cos_a),
                vec3_scale(v_vec, sin_a)
            ));
            
            double tex_u = (double)i / radial_segments;
            double tex_v = (double)circle;
            
            mesh->vertices[vertex_index++] = create_vertex_with_normal(circle_pos, side_normal, tex_u, tex_v);
        }
    }
    
    // Add center vertices for caps
    t_vec3 bottom_center = vec3_add(cylinder_data.center, vec3_scale(axis, -half_height));
    t_vec3 top_center = vec3_add(cylinder_data.center, vec3_scale(axis, half_height));
    
    mesh->vertices[vertex_index++] = create_vertex_with_normal(bottom_center, vec3_scale(axis, -1), 0.5, 0.5);
    mesh->vertices[vertex_index++] = create_vertex_with_normal(top_center, axis, 0.5, 0.5);
    
    int bottom_center_idx = vertex_count - 2;
    int top_center_idx = vertex_count - 1;
    
    // Generate triangles
    int triangle_index = 0;
    
    // Side triangles
    for (int i = 0; i < radial_segments; i++) {
        int next_i = (i + 1) % radial_segments;
        
        int bottom_current = i;
        int bottom_next = next_i;
        int top_current = i + radial_segments;
        int top_next = next_i + radial_segments;
        
        // First triangle of quad
        mesh->triangles[triangle_index++] = (t_triangle){bottom_current, top_current, bottom_next, {0,0,0}, 0};
        
        // Second triangle of quad
        mesh->triangles[triangle_index++] = (t_triangle){bottom_next, top_current, top_next, {0,0,0}, 0};
    }
    
    // Bottom cap triangles
    for (int i = 0; i < radial_segments; i++) {
        int next_i = (i + 1) % radial_segments;
        mesh->triangles[triangle_index++] = (t_triangle){bottom_center_idx, next_i, i, {0,0,0}, 0};
    }
    
    // Top cap triangles
    for (int i = 0; i < radial_segments; i++) {
        int next_i = (i + 1) % radial_segments;
        int top_i = i + radial_segments;
        int top_next = next_i + radial_segments;
        mesh->triangles[triangle_index++] = (t_triangle){top_center_idx, top_i, top_next, {0,0,0}, 0};
    }
    
    mesh->center = cylinder_data.center;
    mesh->radius = sqrt(radius * radius + half_height * half_height);
    mesh->name = strdup("cylinder_mesh");
    
    return mesh;
}

// ============================================================================
// CONVERSION FROM PARSED DATA
// ============================================================================

// Convert your parsed sphere data to mesh
t_mesh *convert_parsed_sphere_to_mesh(char **tokens, int resolution) {
    t_sphere_data sphere_data;
    char **coords, **color_parts;
    
    // Parse "sp 0.0,0.0,20.6 12.6 10,0,255"
    coords = ft_split(tokens[1], ',');
    sphere_data.center.x = ft_atof(coords[0]);
    sphere_data.center.y = ft_atof(coords[1]);
    sphere_data.center.z = ft_atof(coords[2]);
    
    sphere_data.diameter = ft_atof(tokens[2]);
    
    color_parts = ft_split(tokens[3], ',');
    sphere_data.color[0] = ft_atoi(color_parts[0]);
    sphere_data.color[1] = ft_atoi(color_parts[1]);
    sphere_data.color[2] = ft_atoi(color_parts[2]);
    
    free_split(coords);
    free_split(color_parts);
    
    // Convert to mesh with specified resolution
    return sphere_to_mesh(sphere_data, resolution, resolution / 2);
}

// Convert your parsed plane data to mesh
t_mesh *convert_parsed_plane_to_mesh(char **tokens, double size) {
    t_plane_data plane_data;
    char **coords, **normal_parts, **color_parts;
    
    // Parse "pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225"
    coords = ft_split(tokens[1], ',');
    plane_data.point.x = ft_atof(coords[0]);
    plane_data.point.y = ft_atof(coords[1]);
    plane_data.point.z = ft_atof(coords[2]);
    
    normal_parts = ft_split(tokens[2], ',');
    plane_data.normal.x = ft_atof(normal_parts[0]);
    plane_data.normal.y = ft_atof(normal_parts[1]);
    plane_data.normal.z = ft_atof(normal_parts[2]);
    
    color_parts = ft_split(tokens[3], ',');
    plane_data.color[0] = ft_atoi(color_parts[0]);
    plane_data.color[1] = ft_atoi(color_parts[1]);
    plane_data.color[2] = ft_atoi(color_parts[2]);
    
    free_split(coords);
    free_split(normal_parts);
    free_split(color_parts);
    
    return plane_to_mesh(plane_data, size);
}

// Convert your parsed cylinder data to mesh
t_mesh *convert_parsed_cylinder_to_mesh(char **tokens, int resolution) {
    t_cylinder_data cylinder_data;
    char **coords, **axis_parts, **color_parts;
    
    // Parse "cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255"
    coords = ft_split(tokens[1], ',');
    cylinder_data.center.x = ft_atof(coords[0]);
    cylinder_data.center.y = ft_atof(coords[1]);
    cylinder_data.center.z = ft_atof(coords[2]);
    
    axis_parts = ft_split(tokens[2], ',');
    cylinder_data.axis.x = ft_atof(axis_parts[0]);
    cylinder_data.axis.y = ft_atof(axis_parts[1]);
    cylinder_data.axis.z = ft_atof(axis_parts[2]);
    
    cylinder_data.diameter = ft_atof(tokens[3]);
    cylinder_data.height = ft_atof(tokens[4]);
    
    color_parts = ft_split(tokens[5], ',');
    cylinder_data.color[0] = ft_atoi(color_parts[0]);
    cylinder_data.color[1] = ft_atoi(color_parts[1]);
    cylinder_data.color[2] = ft_atoi(color_parts[2]);
    
    free_split(coords);
    free_split(axis_parts);
    free_split(color_parts);
    
    return cylinder_to_mesh(cylinder_data, resolution);
}

// ============================================================================
// SCENE MANAGEMENT
// ============================================================================

// Scene structure to hold all meshes
typedef struct s_mesh_scene {
    t_mesh **meshes;
    int mesh_count;
    int mesh_capacity;
} t_mesh_scene;

// Add mesh to scene
void add_mesh_to_scene(t_mesh_scene *scene, t_mesh *mesh) {
    if (scene->mesh_count >= scene->mesh_capacity) {
        scene->mesh_capacity *= 2;
        scene->meshes = realloc(scene->meshes, sizeof(t_mesh*) * scene->mesh_capacity);
    }
    scene->meshes[scene->mesh_count++] = mesh;
}

// Convert entire parsed scene to meshes
t_mesh_scene *convert_scene_to_meshes(/* your parsed scene data */) {
    t_mesh_scene *scene = malloc(sizeof(t_mesh_scene));
    scene->meshes = malloc(sizeof(t_mesh*) * 10);
    scene->mesh_count = 0;
    scene->mesh_capacity = 10;
    
    // Example usage - you'd iterate through your parsed objects
    /*
    for each parsed sphere:
        t_mesh *sphere_mesh = convert_parsed_sphere_to_mesh(tokens, 32); // 32 segments
        add_mesh_to_scene(scene, sphere_mesh);
    
    for each parsed plane:
        t_mesh *plane_mesh = convert_parsed_plane_to_mesh(tokens, 100.0); // 100x100 size
        add_mesh_to_scene(scene, plane_mesh);
    
    for each parsed cylinder:
        t_mesh *cylinder_mesh = convert_parsed_cylinder_to_mesh(tokens, 24); // 24 radial segments
        add_mesh_to_scene(scene, cylinder_mesh);
    */
    
    return scene;
}

// ============================================================================
// QUALITY CONTROL FUNCTIONS
// ============================================================================

// Calculate mesh quality metrics
void analyze_mesh_quality(t_mesh *mesh) {
    printf("Mesh: %s\n", mesh->name);
    printf("  Vertices: %d\n", mesh->vertex_count);
    printf("  Triangles: %d\n", mesh->triangle_count);
    printf("  Center: (%.2f, %.2f, %.2f)\n", mesh->center.x, mesh->center.y, mesh->center.z);
    printf("  Bounding radius: %.2f\n", mesh->radius);
    
    // Calculate average triangle area for quality assessment
    double total_area = 0.0;
    for (int i = 0; i < mesh->triangle_count; i++) {
        t_triangle *tri = &mesh->triangles[i];
        t_vec3 v1 = mesh->vertices[tri->v1].position;
        t_vec3 v2 = mesh->vertices[tri->v2].position;
        t_vec3 v3 = mesh->vertices[tri->v3].position;
        
        t_vec3 edge1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
        t_vec3 edge2 = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};
        t_vec3 cross = vec3_cross(edge1, edge2);
        double area = sqrt(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z) / 2.0;
        total_area += area;
    }
    printf("  Average triangle area: %.6f\n", total_area / mesh->triangle_count);
    printf("\n");
}
