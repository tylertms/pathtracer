#include "render.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#include "scene.h"
#include "types.h"
#include "vmath.h"
#include "config.h"

/*
  https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
*/
/*float random_state(uint *input) {
  uint state = *input * 747796405u + 2891336453u;
  *input = state;

  uint result = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
  result = (result >> 22u) ^ result;
  return result / 4294967295.0;
}*/

float frand() {
  return (float)rand() / (float)RAND_MAX;
}

float random_normal_distribution(uint *input) {
  float theta = 2 * M_PI * frand();
  float rho = sqrtf(-2 * logf(frand()));
  return rho * cosf(theta);
}

Vec3f random_direction(uint *input) {
  float x = random_normal_distribution(input);
  float y = random_normal_distribution(input);
  float z = random_normal_distribution(input);
  return normalize3f((Vec3f){ x, y, z });
}

Vec3f random_hemisphere_direction(Vec3f normal, uint *input) {
  Vec3f dir = random_direction(input);
  float dot = dot3f(normal, dir);
  return scale3f(dir, signf(dot));
}

// trace the path of the ray from the camera up to MAX_BOUNCES
Vec3f trace(Ray ray, uint *rng_state) {
  Vec3f incoming_light = { 0, 0, 0 };
  Vec3f ray_color = { 255, 255, 255 };

  for (int i = 0; i <= MAX_BOUNCES; i++) {
    Hit hit = ray_collision(ray);

    if (hit.did_hit) {
      ray.origin = hit.hit_pos;
      ray.dir = random_hemisphere_direction(hit.normal, rng_state);
      //printf("random dir: %f, %f, %f, dot = %f\n", ray.dir.x, ray.dir.y, ray.dir.z, dot3f(hit.normal, ray.dir));

      Material mtl = hit.material;
      Vec3f emitted_light = scale3f(mtl.emission_color, mtl.emission_strength);
      incoming_light = add3f(incoming_light, mult_color3f(emitted_light, ray_color));
      ray_color = mult_color3f(ray_color, mtl.color);
    } else {
      break;
    }
  }

  return incoming_light;
}

// stub frag shader function
Vec3c shader(Ray ray, int x, int y) {
  uint rng_state = y * WINDOW_WIDTH + x;
  
  Vec3f total_light = { 0, 0, 0 };
  for (int i = 0; i < SAMPLES; i++) {
    total_light = add3f(total_light, trace(ray, &rng_state));
  }
  total_light = scale3f(total_light, (float)1.f / SAMPLES);
  Vec3c color = to3c(total_light);
  
  return color;
}

void render(SDL_Renderer *renderer, SDL_Window *window) {
  // place all objects in scene before rendering
  scene_init();
  srand(time(NULL));

  // TODO: move cam
  Vec3f cam = {0, 4, -20};

  // iterate through every pixel
  for (int x = 0; x < WINDOW_WIDTH; x++) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
      // create pixel in 3d space with bounds scaled to 1
      float scale = fmaxf(WINDOW_WIDTH, WINDOW_HEIGHT) / 8;

      float x_adj = ((float)x - WINDOW_WIDTH / 2) / scale;
      float y_adj = ((float)y - WINDOW_HEIGHT / 2) / scale;
      Vec3f pixel = {x_adj, y_adj + 4, -5};

      // ray origin is at cam
      // dir is normalized vector to pixel
      Ray ray;
      ray.origin = cam;
      ray.dir = normalize3f(sub3f(pixel, cam));

      // trace the ray out from the camera and get the color, if any
      // printf("origin: %f %f %f, dir: %f %f %f\n", ray.origin.x, ray.origin.y,
      // ray.origin.z, ray.dir.x, ray.dir.y, ray.dir.z);
      Vec3c rgb = shader(ray, x, y);

      // change our draw color appropriately and draw the pixel
      SDL_SetRenderDrawColor(renderer, rgb.x, rgb.y, rgb.z, 255);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }

  // display our renderered image
  SDL_RenderPresent(renderer);
}
