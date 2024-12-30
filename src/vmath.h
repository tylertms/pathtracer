#pragma once
#include "types.h"

float fast_inv_sqrt(float number);

float dot3f(Vec3f a, Vec3f b);
Vec3f add3f(Vec3f a, Vec3f b);
Vec3f sub3f(Vec3f a, Vec3f b);
Vec3f scale3f(Vec3f v, float f);
Vec3f normalize3f(Vec3f v);