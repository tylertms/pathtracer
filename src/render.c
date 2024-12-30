#include <SDL2/SDL.h>
#include "render.h"
#include "types.h"
#include "vmath.h"
#include "intersect.h"

#include <stdio.h>
#include <math.h>

// stub frag shader function
Vec3c color(Ray ray) {
  Hit hit = ray_intersect_sphere(ray, (Sphere){ .center = {0, 0, 0}, .radius = 1 });
  return (Vec3c) { 0, 0, 255 * hit.did_hit };
}

void render(SDL_Renderer *renderer, SDL_Window *window) {
  // get the size of the window to 
  // distribute one ray per pixel
  int width, height;
  SDL_GetWindowSize(window, &width, &height);

  // TODO: move cam
  Vec3f cam = { 0, 0, -2 };

  // iterate through every pixel
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      // create pixel in 3d space with bounds scaled to 1
      float scale = fmaxf(width, height) / 8;
  
      float x_adj = ((float)x - width / 2) / scale;
      float y_adj = ((float)y - height / 2) / scale;
      Vec3f pixel = { x_adj, y_adj, 0 };

      // ray origin is at cam
      // dir is normalized vector to pixel
      Ray ray;
      ray.origin = cam;
      ray.dir = normalize3f(sub3f(pixel, cam));

      // trace the ray out from the camera and get the color, if any
      // printf("origin: %f %f %f, dir: %f %f %f\n", ray.origin.x, ray.origin.y, ray.origin.z, ray.dir.x, ray.dir.y, ray.dir.z);
      Vec3c rgb = color(ray);

      // change our draw color appropriately and draw the pixel
      SDL_SetRenderDrawColor(renderer, rgb.x, rgb.y, rgb.z, 255);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }

  // display our renderered image
  SDL_RenderPresent(renderer);
}

