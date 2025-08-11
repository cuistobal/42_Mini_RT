
static double	aabb_surface(t_aabb box)
{
    double	dx;
    double	dy;
    double	dz;

    dx = box.max.x - box.min.x;
    dy = box.max.y - box.min.y;
    dz = box.max.z - box.min.z;
    return (2.0 * (dx * dy + dx * dz + dy * dz));
}

static inline int compute_cost(int oa, int ob)
{
	return ((oa > ob) - (oa < ob));
}

static int	compare_obj_axis(const void *a, const void *b, void *axis_ptr)
{
    const t_object	*oa;
    const t_object	*ob;
    int				axis;

    oa = *(const t_object **)a;
    ob = *(const t_object **)b;
    axis = *(int *)axis_ptr;
    if (axis == 0)
        return (compute_cost(oa->position.x, oa->position.x));
    if (axis == 1)
        return (compute_cost(oa->position.y, oa->position.y));
    return (compute_cost(oa->position.z, oa->position.z));
}

static void	sort_objects_axis(t_object **objects, int count, int axis)
{
    int	(*cmp)(const void *, const void *, void *);
    cmp = compare_obj_axis;
    ft_qsort(objects, count, sizeof(t_object *), cmp, &axis);
}

// ft_qsort: qsort compatible with extra arg (axis)
void	ft_qsort(void *base, size_t nmemb, size_t size,
            int (*compar)(const void *, const void *, void *), void *arg)
{
    size_t	i;
    size_t	j;
    char	*arr;
    char	tmp[sizeof(void *)];

    arr = (char *)base;
    i = 0;
    while (i < nmemb)
    {
        j = i + 1;
        while (j < nmemb)
        {
            if (compar(arr + i * size, arr + j * size, arg) > 0)
            {
                ft_memcpy(tmp, arr + i * size, size);
                ft_memcpy(arr + i * size, arr + j * size, size);
                ft_memcpy(arr + j * size, tmp, size);
            }
            j++;
        }
        i++;
    }
}

static int	find_sah_split(t_object **objects, int count, int *best_axis, int *best_index)
{
    int		axis;
    int		split;
    int		i;
    double	best_cost;
    double	cost;
    t_aabb	*left_bounds;
    t_aabb	*right_bounds;

    best_cost = INFINITY;
    *best_axis = 0;
    *best_index = 1;

    left_bounds = malloc(sizeof(t_aabb) * count);
    right_bounds = malloc(sizeof(t_aabb) * count);

    axis = 0;
    while (axis < 3)
    {
        sort_objects_axis(objects, count, axis);
        left_bounds[0] = get_object_bounds(objects[0]);
        i = 1;
        while (i < count)
        {
            left_bounds[i] = aabb_union(left_bounds[i - 1],
                    get_object_bounds(objects[i]));
            i++;
        }
        right_bounds[count - 1] = get_object_bounds(objects[count - 1]);
        i = count - 2;
        while (i >= 0)
        {
            right_bounds[i] = aabb_union(right_bounds[i + 1],
                    get_object_bounds(objects[i]));
            i--;
        }
        split = 1;
        while (split < count)
        {
            double	left_area = aabb_surface(left_bounds[split - 1]);
            double	right_area = aabb_surface(right_bounds[split]);
            cost = left_area * split + right_area * (count - split);
            if (cost < best_cost)
            {
                best_cost = cost;
                *best_axis = axis;
                *best_index = split;
            }
            split++;
        }
        axis++;
    }
    free(left_bounds);
    free(right_bounds);
    return (*best_index);
}

static t_bvh_node	*build_bvh_recursive(t_object **objects, int count)
{
    t_bvh_node	*node;
    t_aabb		bounds;
    int			i;
    int			axis;
    int			split;

    if (count <= 0)
        return (NULL);
    node = safe_malloc(sizeof(t_bvh_node));
    if (!node)
        return (NULL);
    bounds = get_object_bounds(objects[0]);
    i = 1;
    while (i < count)
    {
        bounds = aabb_union(bounds, get_object_bounds(objects[i]));
        i++;
    }
    node->bounds = bounds;
    if (count == 1)
    {
        node->object = objects[0];
        node->left = NULL;
        node->right = NULL;
        return (node);
    }
    node->object = NULL;
    axis = 0;
    split = 1;
    if (count > 2)
        find_sah_split(objects, count, &axis, &split);
    sort_objects_axis(objects, count, axis);
    node->left = build_bvh_recursive(objects, split);
    node->right = build_bvh_recursive(objects + split, count - split);
    return (node);
}
