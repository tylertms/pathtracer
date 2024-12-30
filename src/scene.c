#include "scene.h"

#include <math.h>
#include "types.h"
#include "vmath.h"
#include <stdio.h>

void scene_init() {
  Material mtl;;
  mtl.color = (Vec3f){ 220, 50, 220 };

  Sphere s0;
  s0.center = (Vec3f){ 0, 10, 0 };
  s0.radius = 10;
  s0.material = mtl;

  mtl.color = (Vec3f){ 255, 255, 255 };

  Sphere s1;
  s1.center = (Vec3f){ 2, -2, 0 };
  s1.radius = 2;
  s1.material = mtl;

  mtl.color = (Vec3f){ 255, 0, 0 };

  Sphere s2;
  s2.center = (Vec3f){ -1.5, -0.5, -1.5 };
  s2.radius = 1.5;
  s2.material = mtl;

  mtl.color = (Vec3f){ 0, 0, 0 };
  mtl.emission_color = (Vec3f){ 255, 255, 255 };
  mtl.emission_strength = 10;

  Sphere s3;
  s3.center = (Vec3f){ -3, 0, 10 };
  s3.radius = 8;
  s3.material = mtl;

  spheres[0] = s0;
  spheres[1] = s1;
  spheres[2] = s2;
  spheres[3] = s3;
}

// get hit data for the first collision of a ray
Hit ray_collision(Ray ray) {
  Hit closest_hit = (Hit){ .dist = MAXFLOAT };

  // loop through all objects in the scene
  for (int i = 0; i < NUM_SPHERES; i++) {
    // get hit data about particular intersection
    Sphere sphere = spheres[i];
    Hit hit = ray_intersect_sphere(ray, sphere);

    // replace hit data if the collision comes before the current
    if (hit.did_hit && hit.dist < closest_hit.dist) {
      closest_hit = hit;
      closest_hit.material = sphere.material;
    }
  }

  return closest_hit;
}

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