#include "intersect.h"

#include <math.h>
#include "types.h"
#include "vmath.h"

Hit ray_intersect_sphere(Ray ray, Sphere sphere) {
  Hit hit = {.did_hit = 0};
  Vec3f offset = sub3f(ray.origin, sphere.center);

  // quadratic formula params
  float a = dot3f(ray.dir, ray.dir);
  float b = 2 * dot3f(offset, ray.dir);
  float c = dot3f(offset, offset) - sphere.radius * sphere.radius;

  float discriminant = b * b - 4 * a * c;

  // if there is a solution
  if (discriminant >= 0) {
    // take only the one with shorter distance (first hit)
    float dist = (-b - sqrtf(discriminant)) / (2 * a);

    // populate hit data
    if (dist >= 0) {
      hit.did_hit = 1;
      hit.dist = dist;
      hit.hit_pos = add3f(ray.origin, scale3f(ray.dir, dist));
      hit.normal = normalize3f(sub3f(hit.hit_pos, sphere.center));
    }
  }

  return hit;
}