#pragma once
#include <stdint.h>

typedef struct {
  float x, y, z;
} Vec3f;

typedef struct {
  uint8_t x, y, z;
} Vec3c;

typedef struct {
  Vec3c color;
} Material;

typedef struct {
  Vec3f center;
  float radius;
  Material mtl;
} Sphere;

typedef struct {
  Vec3f origin;
  Vec3f dir;
} Ray;

typedef struct {
  int did_hit;
  float dist;
  Vec3f hit_pos;
  Vec3f normal;
} Hit;