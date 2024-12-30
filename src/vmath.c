#include "vmath.h"
#include "types.h"
#include <stdint.h>

/*
  https://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
float fast_inv_sqrt(float n) {
  union {
    float    f;
    uint32_t i;
  } conv = { .f = n };
  conv.i  = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5F - (n * 0.5F * conv.f * conv.f);
  return conv.f;
}

float dot3f(Vec3f a, Vec3f b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3f add3f(Vec3f a, Vec3f b) {
  return (Vec3f){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3f sub3f(Vec3f a, Vec3f b) {
  return (Vec3f){ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3f scale3f(Vec3f v, float f) {
  return (Vec3f){ v.x * f, v.y * f, v.z * f };
}

Vec3f normalize3f(Vec3f v) {
  return scale3f(v, fast_inv_sqrt(dot3f(v, v)));
}