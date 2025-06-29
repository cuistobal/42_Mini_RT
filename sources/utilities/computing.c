#include "minirt.h"

//This function takes a point (x,y,z) as parameter and return it's floated 
//normalization.
inline float	findDistance(int *arr)
{
    return (sqrtf(powf(arr[X], 2) + powf(arr[Y], 2) + powf(arr[Z], 2)));
}

//This function computes the scalar product of 2 objects, hence giving us the
//distance between those 2 bad bois.
inline float	computeScalarProduct(int *obj1, int *obj2)
{
    return (findDistance(obj1) - findDistance(obj2));
}
