#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// --- Vecteurs ---
typedef struct vector {
    float x, y, z;
} t_vec;

t_vec vec3_add(t_vec a, t_vec b) { return (t_vec){a.x + b.x, a.y + b.y, a.z + b.z}; }
t_vec vec3_sub(t_vec a, t_vec b) { return (t_vec){a.x - b.x, a.y - b.y, a.z - b.z}; }
t_vec vec3_scale(t_vec v, float s) { return (t_vec){v.x * s, v.y * s, v.z * s}; }
float vec3_dot(t_vec a, t_vec b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float vec3_norm(t_vec v) { return sqrtf(vec3_dot(v, v)); }
t_vec vec3_normalized(t_vec v) { float n = vec3_norm(v); return vec3_scale(v, 1.0f / n); }
t_vec vec3_negate(t_vec v) { return (t_vec){-v.x, -v.y, -v.z}; }
t_vec vec3_cross(t_vec a, t_vec b) {
    return (t_vec){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// --- Matériau ---
typedef struct {
    float refractive_index;
    float albedo[4];
    t_vec diffuse_color;
    float specular_exponent;
} t_material;

// --- Structures objets polymorphes ---
typedef struct s_obj t_obj;
typedef struct s_sphere {
    t_vec center;
    float radius;
    t_material material;
} t_sphere;

// Résultat d'intersection générique
typedef struct {
    bool hit;
    float distance;
    t_vec point;
    t_vec normal;
    t_material material;
} t_hit;

// Prototype de fonction d'intersection
typedef bool (*t_intersect_fn)(t_obj *self, t_vec orig, t_vec dir, t_hit *hit);

// Objet générique
struct s_obj {
    char type;
    void *data;
    t_intersect_fn intersect;
    // Tu peux ajouter d'autres pointeurs de fonctions ici (get_normal, get_color, etc.)
    t_obj *next;
};

// --- Fonctions d'intersection pour chaque type d'objet ---
bool sphere_intersect(t_obj *self, t_vec orig, t_vec dir, t_hit *hit) {
    t_sphere *s = (t_sphere *)self->data;
    t_vec L = vec3_sub(s->center, orig);
    float tca = vec3_dot(L, dir);
    float d2 = vec3_dot(L, L) - tca * tca;
    float r2 = s->radius * s->radius;
    if (d2 > r2) return false;
    float thc = sqrtf(r2 - d2);
    float t0 = tca - thc, t1 = tca + thc;
    float t = (t0 > 0.001f) ? t0 : (t1 > 0.001f ? t1 : -1);
    if (t < 0) return false;
    hit->hit = true;
    hit->distance = t;
    hit->point = vec3_add(orig, vec3_scale(dir, t));
    hit->normal = vec3_normalized(vec3_sub(hit->point, s->center));
    hit->material = s->material;
    return true;
}

// --- Création d'un objet sphère ---
t_obj *create_sphere(t_vec center, float radius, t_material mat) {
    t_obj *obj = malloc(sizeof(t_obj));
    t_sphere *s = malloc(sizeof(t_sphere));
    *s = (t_sphere){center, radius, mat};
    obj->type = 's';
    obj->data = s;
    obj->intersect = sphere_intersect;
    obj->next = NULL;
    return obj;
}

// --- Liste d'objets (scène) ---
typedef struct {
    t_obj *objects;
    t_vec *lights;
    size_t light_count;
} t_scene;

// --- Intersection scène (polymorphe) ---
bool scene_intersect(t_scene *scene, t_vec orig, t_vec dir, t_hit *closest_hit) {
    bool found = false;
    float min_dist = 1e30;
    for (t_obj *obj = scene->objects; obj; obj = obj->next) {
        t_hit hit = {0};
        if (obj->intersect(obj, orig, dir, &hit) && hit.distance < min_dist) {
            min_dist = hit.distance;
            *closest_hit = hit;
            found = true;
        }
    }
    return found;
}

// --- Raytracing principal ---
t_vec reflect(t_vec I, t_vec N) { return vec3_sub(I, vec3_scale(N, 2.0f * vec3_dot(I, N))); }
t_vec refract(t_vec I, t_vec N, float eta_t, float eta_i) {
    float cosi = -fmaxf(-1.0f, fminf(1.0f, vec3_dot(I, N)));
    if (cosi < 0) return refract(I, vec3_negate(N), eta_i, eta_t);
    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? (t_vec){1, 0, 0} : vec3_add(vec3_scale(I, eta), vec3_scale(N, eta * cosi - sqrtf(k)));
}

t_vec cast_ray(t_scene *scene, t_vec orig, t_vec dir, int depth) {
    if (depth > 4) return (t_vec){0.2, 0.7, 0.8};
    t_hit hit = {0};
    if (!scene_intersect(scene, orig, dir, &hit))
        return (t_vec){0.2, 0.7, 0.8};

    t_vec reflect_dir = vec3_normalized(reflect(dir, hit.normal));
    t_vec refract_dir = vec3_normalized(refract(dir, hit.normal, hit.material.refractive_index, 1.0f));
    t_vec reflect_color = cast_ray(scene, hit.point, reflect_dir, depth + 1);
    t_vec refract_color = cast_ray(scene, hit.point, refract_dir, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (size_t i = 0; i < scene->light_count; i++) {
        t_vec light_dir = vec3_normalized(vec3_sub(scene->lights[i], hit.point));
        t_hit shadow_hit = {0};
        if (scene_intersect(scene, hit.point, light_dir, &shadow_hit) &&
            vec3_norm(vec3_sub(shadow_hit.point, hit.point)) < vec3_norm(vec3_sub(scene->lights[i], hit.point)))
            continue;
        diffuse_light_intensity += fmaxf(0.0f, vec3_dot(light_dir, hit.normal));
        specular_light_intensity += powf(fmaxf(0.0f, -vec3_dot(reflect(vec3_negate(light_dir), hit.normal), dir)), hit.material.specular_exponent);
    }

    t_vec diffuse = vec3_scale(hit.material.diffuse_color, diffuse_light_intensity * hit.material.albedo[0]);
    t_vec specular = vec3_scale((t_vec){1.0, 1.0, 1.0}, specular_light_intensity * hit.material.albedo[1]);
    t_vec reflection = vec3_scale(reflect_color, hit.material.albedo[2]);
    t_vec refraction = vec3_scale(refract_color, hit.material.albedo[3]);

    return vec3_add(vec3_add(diffuse, specular), vec3_add(reflection, refraction));
}

// --- Main ---
int main() {
    // Matériaux
    t_material ivory = {1.0, {0.9, 0.5, 0.1, 0.0}, {0.4, 0.4, 0.3}, 50.0};
    t_material glass = {1.5, {0.0, 0.9, 0.1, 0.8}, {0.6, 0.7, 0.8}, 125.0};
    t_material red_rubber = {1.0, {1.4, 0.3, 0.0, 0.0}, {0.3, 0.1, 0.1}, 10.0};
    t_material mirror = {1.0, {0.0, 16.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.0};

    // Scène
    t_scene scene = {0};
    scene.objects = create_sphere((t_vec){-3, 0, -16}, 2, ivory);
    scene.objects->next = create_sphere((t_vec){-1.0, -1.5, -12}, 2, glass);
    scene.objects->next->next = create_sphere((t_vec){1.5, -0.5, -18}, 3, red_rubber);
    scene.objects->next->next->next = create_sphere((t_vec){7, 5, -18}, 4, mirror);

    t_vec lights[] = {{-20, 20, 20}, {30, 50, -25}, {30, 20, 30}};
    scene.lights = lights;
    scene.light_count = sizeof(lights) / sizeof(t_vec);

    // Framebuffer
    const int width = 1024, height = 768;
    const float fov = 1.05f;
    t_vec *framebuffer = malloc(width * height * sizeof(t_vec));

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float dir_x = (i + 0.5f) - width / 2.0f;
            float dir_y = -(j + 0.5f) + height / 2.0f;
            float dir_z = -height / (2.0f * tanf(fov / 2.0f));
            t_vec dir = vec3_normalized((t_vec){dir_x, dir_y, dir_z});
            framebuffer[j * width + i] = cast_ray(&scene, (t_vec){0, 0, 0}, dir, 0);
        }
    }

    FILE *ofs = fopen("out.ppm", "wb");
    fprintf(ofs, "P6\n%d %d\n255\n", width, height);
    for (int i = 0; i < width * height; i++) {
        t_vec color = framebuffer[i];
        float max = fmaxf(1.0f, fmaxf(color.x, fmaxf(color.y, color.z)));
        unsigned char r = (unsigned char)(255 * color.x / max);
        unsigned char g = (unsigned char)(255 * color.y / max);
        unsigned char b = (unsigned char)(255 * color.z / max);
        fwrite(&r, 1, 1, ofs);
        fwrite(&g, 1, 1, ofs);
        fwrite(&b, 1, 1, ofs);
    }
    fclose(ofs);
    free(framebuffer);

    // Libération mémoire des objets
    t_obj *obj = scene.objects;
    while (obj) {
        t_obj *next = obj->next;
        free(obj->data);
        free(obj);
        obj = next;
    }
    return 0;
}

