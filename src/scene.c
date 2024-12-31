#include "scene.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "types.h"
#include "vmath.h"

Hit hit_cache[WINDOW_WIDTH * WINDOW_HEIGHT];

void scene_init() {
  for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    hit_cache[i].did_hit = -1;
  }

  Sphere s0;
  s0.center = (Vec3f){0, 36, 0};
  s0.radius = 30;
  s0.material = (Material){
      .color = (Vec3f){45.0 / 255, 181.0 / 255, 54.0 / 255}, .roughness = 1};

  Sphere s1;
  s1.center = (Vec3f){2, 4.2, 0};
  s1.radius = 2;
  s1.material = (Material){.color = (Vec3f){1.0f, 1.0f, 1.0f}, .roughness = 1};

  Sphere s2;
  s2.center = (Vec3f){-0.6, 4.7, -2.8};
  s2.radius = 1.5;
  s2.material = (Material){.color = (Vec3f){1.0f, 0.1f, 0.1f}, .roughness = 1};

  Sphere s3;
  s3.center = (Vec3f){-100, -40, 60};
  s3.radius = 50;
  s3.material = (Material){
      .color = (Vec3f){0.0f, 0.0f, 0.0f},
      .emission_color = (Vec3f){255.0 / 255, 225.0 / 255, 168.0 / 255},
      .emission_strength = 10,
      .roughness = 1};

  Sphere s4;
  s4.center = (Vec3f){-1, 2.5, 3.5};
  s4.radius = 1.5;
  s4.material = (Material){.color = (Vec3f){1, 1, 1}, .roughness = 0};

  spheres[0] = s0;
  spheres[1] = s1;
  spheres[2] = s2;
  spheres[3] = s3;
  // spheres[4] = s4;
}

void scene_deinit() {

}

// get hit data for the first collision of a ray
Hit ray_collision(const Ray *ray, int collision_number, int x, int y) {
  int pixel_index = y * WINDOW_WIDTH + x;

  if (collision_number == 0 && hit_cache[pixel_index].did_hit >= 0) {
    return hit_cache[pixel_index];
  }

  Hit closest_hit = (Hit){ .did_hit = 0, .dist = MAXFLOAT };

  // loop through all objects in the scene
  for (int i = 0; i < NUM_SPHERES; i++) {
    // get hit data about particular intersection
    Hit hit = ray_intersect_sphere(ray, &spheres[i]);

    // replace hit data if the collision comes before the current
    if (hit.did_hit && hit.dist < closest_hit.dist) {
      closest_hit = hit;
      closest_hit.material = spheres[i].material;
    }
  }

  if (collision_number == 0)
    hit_cache[pixel_index] = closest_hit;

  return closest_hit;
}

int overlap = 0;
Hit ray_intersect_sphere(const Ray *ray, const Sphere *sphere) {
  Hit hit = {.did_hit = 0};
  Vec3f offset = sub3f(ray->origin, sphere->center);

  float a = dot3f(ray->dir, ray->dir);
  float b = 2 * dot3f(offset, ray->dir);
  float c = dot3f(offset, offset) - sphere->radius * sphere->radius;

  float discriminant = b * b - 4 * a * c;

  if (discriminant >= 0) {
    float dist = (-b - sqrtf(discriminant)) / (2 * a);

    // populate hit data
    if (dist >= 1e-4) {
      hit.did_hit = 1;
      hit.dist = dist;
      hit.hit_pos = add3f(ray->origin, scale3f(ray->dir, dist));
      hit.normal =
          scale3f((sub3f(hit.hit_pos, sphere->center)), 1.0f / sphere->radius);

    }
  }

  return hit;
}