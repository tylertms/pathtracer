#pragma once
#include "types.h"

#define NUM_SPHERES 4
Sphere spheres[NUM_SPHERES];

void scene_init();
void scene_deinit();
Hit ray_collision(const Ray *ray, int collision_number, int x, int y);
Hit ray_intersect_sphere(const Ray *ray, const Sphere *sphere);