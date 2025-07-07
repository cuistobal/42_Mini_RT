#include "minirt.h"

static bool intersect_aabb(t_aabb bounds, t_vec origin, t_vec dir, float *t_min)
{
    t_vec inv_dir = {1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z};
    t_vec t1, t2;

    t1 = vec_scale(vec_sub(bounds.min_vec, origin), inv_dir.x);
    t1.y = (bounds.min_vec.y - origin.y) * inv_dir.y;
    t1.z = (bounds.min_vec.z - origin.z) * inv_dir.z;

    t2 = vec_scale(vec_sub(bounds.max_vec, origin), inv_dir.x);
    t2.y = (bounds.max_vec.y - origin.y) * inv_dir.y;
    t2.z = (bounds.max_vec.z - origin.z) * inv_dir.z;

    float tx_min = fminf(t1.x, t2.x);
    float tx_max = fmaxf(t1.x, t2.x);
    float ty_min = fminf(t1.y, t2.y);
    float ty_max = fmaxf(t1.y, t2.y);
    float tz_min = fminf(t1.z, t2.z);
    float tz_max = fmaxf(t1.z, t2.z);

    float tmin_final = fmaxf(fmaxf(tx_min, ty_min), tz_min);
    float tmax_final = fminf(fminf(tx_max, ty_max), tz_max);

    *t_min = tmin_final;
    return tmax_final >= tmin_final && tmin_final < *t_min;
}

bool traverse_bvh(t_bvh *node, t_vec origin, t_vec dir, t_hit *closest_hit)
{
    if (!node)
        return (false);

    float t_min = 1e30f;
    if (!intersect_aabb(node->bounds, origin, dir, &t_min) || t_min > closest_hit->distance)
        return (false);

    bool hit_found = false;

    // Leaf node - check all objects
    if (node->objects)
    {
        t_object *current = node->objects;
        t_hit hit;

        while (current)
        {
            init_hit_values(&hit);
            if (current->methods.intersect(current, origin, dir, &hit) && hit.distance < closest_hit->distance)
            {
                *closest_hit = hit;
                hit_found = true;
            }
            current = current->next;
        }
        return hit_found;
    }

    // Interior node - traverse children
    bool hit_left = traverse_bvh(node->left, origin, dir, closest_hit);
    bool hit_right = traverse_bvh(node->right, origin, dir, closest_hit);

    return hit_left || hit_right;
}
