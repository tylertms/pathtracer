#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "config.h"
#include "render.h"
#include "scene.h"
#include "vmath.h"

int main() {
  struct timeval stop, start;
  gettimeofday(&start, NULL);

  scene_init();

  // init all the SDL stuff
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  int result = SDL_Init(SDL_INIT_VIDEO);
  if (result < 0) {
    SDL_Log("SDL_Init error: %s", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow(TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
    return -1;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
    return -1;
  }

  SDL_Log("SDL2_INITIALIZED");
  SDL_Event event;

  int frame = 0;
  // Allocate an image buffer
  Vec3f *image = (Vec3f *)calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(Vec3f));
  if (!image) {
    fprintf(stderr, "Failed to allocate image buffer.\n");
    return -1;
  }

  // display rendered image until quit is requested
  int done = 0;
  while (1) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
        SDL_Log("SDL2_EVENT_QUIT");
        goto shutdown;
      }
    }

    if (frame < MAX_FRAMES)
      // progressively render SAMPLES_PER_FRAME 
      // to approach the actual image up until MAX_FRAMES
      render(renderer, image, ++frame);
    else {
      if (!done) {
        done = 1;
        gettimeofday(&stop, NULL);
        float seconds = (float)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
        printf("RENDER COMPLETE after %fs\n", seconds);
      }
    }

    SDL_Delay(1);
  }

shutdown:
  SDL_Log("SDL2_SHUTDOWN");

  free(image);
  scene_deinit();

  // deinit SDL stuff
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
