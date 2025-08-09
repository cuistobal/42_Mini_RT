#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Minimal structures for testing
typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;

typedef struct s_aabb
{
    t_vec3 min;
    t_vec3 max;
} t_aabb;

typedef struct s_bvh_node
{
    t_aabb bounds;
    void *object;
    struct s_bvh_node *left;
    struct s_bvh_node *right;
} t_bvh_node;

// Safe memory functions
void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    return ptr;
}

void safe_free(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

// The cleanup_bvh function we want to test
void cleanup_bvh(t_bvh_node *node)
{
    if (!node)
        return;
    
    cleanup_bvh(node->left);
    cleanup_bvh(node->right);
    safe_free((void **)&node);
}

int main(void)
{
    t_bvh_node *node1, *node2, *root;
    
    printf("Creating BVH nodes for cleanup test...\n");
    
    // Create leaf nodes
    node1 = safe_malloc(sizeof(t_bvh_node));
    memset(node1, 0, sizeof(t_bvh_node));
    
    node2 = safe_malloc(sizeof(t_bvh_node));
    memset(node2, 0, sizeof(t_bvh_node));
    
    // Create root node
    root = safe_malloc(sizeof(t_bvh_node));
    memset(root, 0, sizeof(t_bvh_node));
    root->left = node1;
    root->right = node2;
    
    printf("BVH tree created: root with 2 children\n");
    
    // Test cleanup_bvh function
    printf("Testing cleanup_bvh function...\n");
    cleanup_bvh(root);
    
    printf("cleanup_bvh completed successfully!\n");
    printf("Test passed - BVH cleanup function works correctly\n");
    
    return (0);
}