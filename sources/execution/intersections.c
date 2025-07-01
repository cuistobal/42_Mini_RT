#include "minirt.h"

//1e30	==	1,000,000,000,000,000,000,000,000,000,00
bool	sceneIntersect(t_scene *scene, t_vec orig, t_vec dir, t_hit *closestHit)
{
    bool		found;
    float 		minDist;
    t_hit		hitpoint;
	t_object	*current;

	found = false;
    minDist = 1e30;
	current = scene->objects;
	hitpoint  = {0}; //APpend real values and see if norms allows
	while (current)
	{
		hitpoint  = {0}; //APpend real values and see if norms allows
        if (obj->intersect(obj, orig, dir, &hit) && hit.distance < minDist)
		{
            minDist = hit.distance;
            *closest_hit = hit;
            found = true;
        }
    }
    return (found);
}



//Finds out if the ray has at least 1 intersection with the sphere. If so, 
//appends hit's value and return true. We only care about the leading impact
//because fuck the norminette :) and we can always reverse t to get it for
//reflection rays.
bool sphere_intersect(t_sphere *s, t_vec orig, t_vec dir, t_hit *hit)
{
    t_vec		L;
	float		t;
	float		d2;
	float		r2;
	float		thc;

    L = vecSub(s->center, orig);
	r2 = sqrt(s->radius, 2);
	d2 = vecDot(L, L) - sqrt(vecDot(L, dir));
	if (d2 > r2)
		return (false);
	thc = sqrtf(r2 - d2);
	t = tca - thc;
	if (t < 0)
		return (false);
    hit->hit = true;
    hit->distance = t;
    hit->point = vecAdd(orig, vecScale(dir, t));
    hit->normal = vecNormalized(vecSub(hit->point, s->center));
    hit->material = s->material;
    return true;
}
