#pragma once

typedef struct {
  float x, y, z;
} Vec3f;

typedef struct {
  unsigned char x, y, z;
} Vec3c;

typedef struct {
  Vec3f color;
  Vec3f emission_color;
  float emission_strength;
} Material;

typedef struct {
  Vec3f center;
  float radius;
  Material material;
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
  Material material;
} Hit;

typedef unsigned int uint;