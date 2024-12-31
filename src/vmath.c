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

float signf(float n) {
  return n < 0 ? -1 : 1;
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

Vec3f mult_color3f(Vec3f a, Vec3f b) {
  return (Vec3f){ a.x * b.x, a.y * b.y, a.z * b.z };
}

Vec3f scale3f(Vec3f v, float f) {
  return (Vec3f){ v.x * f, v.y * f, v.z * f };
}


Vec3f normalize3f(Vec3f v) {
  return scale3f(v, fast_inv_sqrt(dot3f(v, v)));
}

Vec3f reflect3f(Vec3f v, Vec3f n) {
  return sub3f(v, scale3f(n, 2 * dot3f(v, n)));
}

Vec3c add3c(Vec3c a, Vec3c b) {
  return (Vec3c){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3c to3c(Vec3f v) {
  v = scale3f(v, 255);
  if (v.x > 255) v.x = 255;
  if (v.y > 255) v.y = 255;
  if (v.z > 255) v.z = 255;
  return (Vec3c){ (unsigned char)v.x, (unsigned char)v.y, (unsigned char)v.z };
}

Vec3c scale3c(Vec3c v, float f) {
  return (Vec3c){ (unsigned char)(v.x * f), (unsigned char)(v.y * f), (unsigned char)(v.z * f) };
}