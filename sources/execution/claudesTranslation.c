// Structure pour un nœud de liste chaînée de sphères
typedef struct SphereNode {
    Sphere sphere;
    struct SphereNode *next;
} SphereNode;

// Fonction pour compter les éléments d'une liste
int count_spheres(SphereNode *head) {
    int count = 0;
    SphereNode *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Fonction pour créer l'AABB d'une liste de sphères
AABB create_aabb_from_sphere_list(SphereNode *head) {
    AABB bounds = create_empty_aabb();
    SphereNode *current = head;
    while (current != NULL) {
        bounds = combine_aabb(bounds, create_aabb_from_sphere(&current->sphere));
        current = current->next;
    }
    return bounds;
}

// Fonction pour partitionner la liste selon un axe et une valeur de split
void partition_sphere_list(SphereNode *head, int axis, float split, 
                          SphereNode **left_list, SphereNode **right_list) {
    *left_list = NULL;
    *right_list = NULL;
    SphereNode *left_tail = NULL;
    SphereNode *right_tail = NULL;
    
    SphereNode *current = head;
    while (current != NULL) {
        SphereNode *next = current->next;
        current->next = NULL;
        
        float center = 0;
        switch (axis) {
            case 0: center = current->sphere.center.x; break;
            case 1: center = current->sphere.center.y; break;
            case 2: center = current->sphere.center.z; break;
        }
        
        if (center < split) {
            // Ajouter à la liste de gauche
            if (*left_list == NULL) {
                *left_list = current;
                left_tail = current;
            } else {
                left_tail->next = current;
                left_tail = current;
            }
        } else {
            // Ajouter à la liste de droite
            if (*right_list == NULL) {
                *right_list = current;
                right_tail = current;
            } else {
                right_tail->next = current;
                right_tail = current;
            }
        }
        
        current = next;
    }
}

// Fonction pour évaluer le SAH sur une liste chaînée
float evaluate_sah_list(SphereNode *head, int axis, float split) {
    int left_count = 0, right_count = 0;
    AABB left_bounds = create_empty_aabb();
    AABB right_bounds = create_empty_aabb();
    
    SphereNode *current = head;
    while (current != NULL) {
        float center = 0;
        switch (axis) {
            case 0: center = current->sphere.center.x; break;
            case 1: center = current->sphere.center.y; break;
            case 2: center = current->sphere.center.z; break;
        }
        
        AABB sphere_aabb = create_aabb_from_sphere(&current->sphere);
        
        if (center < split) {
            left_count++;
            left_bounds = combine_aabb(left_bounds, sphere_aabb);
        } else {
            right_count++;
            right_bounds = combine_aabb(right_bounds, sphere_aabb);
        }
        
        current = current->next;
    }
    
    if (left_count == 0 || right_count == 0) {
        return INFINITY;
    }
    
    float left_area = calculate_aabb_surface_area(left_bounds);
    float right_area = calculate_aabb_surface_area(right_bounds);
    
    return left_count * left_area + right_count * right_area;
}

BVHNode *build_bvh_node_list(SphereNode *sphere_list, int depth) {
    if (sphere_list == NULL) {
        return NULL;
    }
    
    BVHNode *node = (BVHNode *)malloc(sizeof(BVHNode));
    node->bounds = create_aabb_from_sphere_list(sphere_list);
    
    int num_spheres = count_spheres(sphere_list);
    
    // Condition d'arrêt : feuille
    if (num_spheres <= 1 || depth >= 40) {
        node->left = node->right = NULL;
        node->sphere = &sphere_list->sphere;  // Pointeur vers la première sphère
        node->sphere_count = num_spheres;
        return node;
    }
    
    // Recherche de la meilleure division avec SAH
    float best_cost = INFINITY;
    int best_axis = 0;
    float best_split = 0;
    
    for (int axis = 0; axis < 3; axis++) {
        for (int i = 1; i < 8; i++) {
            float split;
            if (axis == 0) {
                split = node->bounds.min.x + (i / 8.0f) * (node->bounds.max.x - node->bounds.min.x);
            } else if (axis == 1) {
                split = node->bounds.min.y + (i / 8.0f) * (node->bounds.max.y - node->bounds.min.y);
            } else {
                split = node->bounds.min.z + (i / 8.0f) * (node->bounds.max.z - node->bounds.min.z);
            }
            
            float cost = evaluate_sah_list(sphere_list, axis, split);
            if (cost < best_cost) {
                best_cost = cost;
                best_axis = axis;
                best_split = split;
            }
        }
    }
    
    // Partitionnement de la liste
    SphereNode *left_list = NULL;
    SphereNode *right_list = NULL;
    partition_sphere_list(sphere_list, best_axis, best_split, &left_list, &right_list);
    
    // Si la partition a échoué, créer une feuille
    if (left_list == NULL || right_list == NULL) {
        node->left = node->right = NULL;
        node->sphere = &sphere_list->sphere;
        node->sphere_count = num_spheres;
        return node;
    }
    
    // Récursion sur les sous-listes
    node->left = build_bvh_node_list(left_list, depth + 1);
    node->right = build_bvh_node_list(right_list, depth + 1);
    node->sphere = NULL;
    node->sphere_count = 0;
    
    return node;
}

// Fonction utilitaire pour convertir un tableau en liste chaînée
SphereNode *array_to_sphere_list(Sphere *spheres, int count) {
    if (count <= 0) return NULL;
    
    SphereNode *head = NULL;
    SphereNode *tail = NULL;
    
    for (int i = 0; i < count; i++) {
        SphereNode *new_node = (SphereNode *)malloc(sizeof(SphereNode));
        new_node->sphere = spheres[i];
        new_node->next = NULL;
        
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    return head;
}

// Fonction pour libérer la mémoire d'une liste de sphères
void free_sphere_list(SphereNode *head) {
    while (head != NULL) {
        SphereNode *temp = head;
        head = head->next;
        free(temp);
    }
}
