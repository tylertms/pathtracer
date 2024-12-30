#pragma once
#include "types.h"

#define NUM_SPHERES 4
Sphere spheres[NUM_SPHERES];

void scene_init();
Hit ray_collision(Ray ray);
Hit ray_intersect_sphere(Ray ray, Sphere sphere);