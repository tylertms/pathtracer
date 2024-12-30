#include <SDL2/SDL.h>
#include <stdio.h>

#include "vmath.h"
#include "render.h"
#include "config.h"

int main() {
  // init all the SDL stuff

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  int result = SDL_Init(SDL_INIT_VIDEO);
  if (result < 0) {
    SDL_Log("SDL_Init error: %s", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow(TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
    return -1;
  }

  SDL_Log("SDL2_INITIALIZED");
  SDL_Event event;

  // run our render once
  render(renderer, window);

  // display rendered image until quit is requested
  while (1) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
        SDL_Log("SDL2_EVENT_QUIT");
        goto shutdown;
      }
    }

    //render(renderer, window);
    SDL_Delay(1);
  }

  shutdown: SDL_Log("SDL2_SHUTDOWN");

  // deinit SDL stuff
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

