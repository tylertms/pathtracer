#pragma once
#include <SDL2/SDL.h>
#include "types.h"

#define TABLE_SIZE (1 << 14)
float normal_table[TABLE_SIZE];

float frand();
void initialize_normal_table();
float random_normal_distribution();
Vec3f random_direction();
Vec3f trace(Ray ray, int x, int y);
Vec3f shader(Ray ray, int x, int y);
void *render_thread(void *arg);

void render(SDL_Renderer *renderer, Vec3f *image, int frame);