#pragma once
#include "types.h"

/* ----- GENERAL ----- */
float fast_inv_sqrt(float n);
float signf(float n);

/* ----- FLOAT VECTOR ----- */
float dot3f(Vec3f a, Vec3f b);
Vec3f add3f(Vec3f a, Vec3f b);
Vec3f sub3f(Vec3f a, Vec3f b);
Vec3f mult_color3f(Vec3f a, Vec3f b);
Vec3f scale3f(Vec3f v, float f);
Vec3f normalize3f(Vec3f v);

/* ----- U_CHAR VECTOR ----- */
Vec3c add3c(Vec3c a, Vec3c b);
Vec3c to3c(Vec3f v);
Vec3c mult_color3c(Vec3c a, Vec3c b);
Vec3c scale3c(Vec3c v, float f);