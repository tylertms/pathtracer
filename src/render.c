#include "render.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>

#include "config.h"
#include "scene.h"
#include "types.h"
#include "vmath.h"

int table_index = 0;
int table_initialized = 0;

// per-thread rendering function
// performs path tracing by sampling its assigned region
void *render_thread(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  Vec3f *image = data->image;
  Vec3f cam = data->cam;

  for (int y = data->start_y; y < data->end_y; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      // Create pixel position in 3D space
      float scale = fmaxf(WINDOW_WIDTH, WINDOW_HEIGHT) / 8.0f;
      float x_adj = ((float)x - WINDOW_WIDTH / 2.0f) / scale;
      float y_adj = ((float)y - WINDOW_HEIGHT / 2.0f) / scale;
      Vec3f pixel = {x_adj, y_adj + 1.4, -8.0f};

      // Define the ray from the camera to the pixel
      Ray ray;
      ray.origin = cam;
      ray.dir = normalize3f(sub3f(pixel, cam));

      // Compute the color using the shader
      Vec3f rgb = shader(ray, x, y);

      // Store the color in the image buffer
      Vec3f old_rgb = image[y * WINDOW_WIDTH + x];
      float weight = 1.0f / (data->frame);
      Vec3f avg = add3f(scale3f(old_rgb, 1 - weight), scale3f(rgb, weight));

      image[y * WINDOW_WIDTH + x] = avg;
    }
  }

  pthread_exit(NULL);
}

// render one new frame using SAMPLES_PER_FRAME
// samples per pixel of the window
void render(SDL_Renderer *renderer, Vec3f *image, int frame) {
  if (!table_initialized) initialize_normal_table();
  srand((unsigned int)time(NULL));

  // Define camera position
  Vec3f cam = {0, -2, -20};

  // Create thread data structures
  pthread_t threads[THREADS];
  ThreadData thread_data[THREADS];
  int rows_per_thread = WINDOW_HEIGHT / THREADS;
  int remaining_rows = WINDOW_HEIGHT % THREADS;
  
  for (int i = 0; i < THREADS; i++) {
    thread_data[i].thread_id = i;
    thread_data[i].start_y = i * rows_per_thread;
    thread_data[i].end_y = (i + 1) * rows_per_thread;
    thread_data[i].cam = cam;
    thread_data[i].image = image;
    thread_data[i].frame = frame;

    // Handle any remaining rows in the last thread
    if (i == THREADS - 1) {
      thread_data[i].end_y += remaining_rows;
    }

    int rc = pthread_create(&threads[i], NULL, render_thread, (void *)&thread_data[i]);
    if (rc) {
      fprintf(stderr, "Error: Unable to create thread %d, return code %d\n", i, rc);
      return;
    }
  }

  // Wait for all threads to complete
  for (int i = 0; i < THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Render the image buffer to the screen
  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      Vec3c rgb = to3c(image[y * WINDOW_WIDTH + x]);
      SDL_SetRenderDrawColor(renderer, rgb.x, rgb.y, rgb.z, 255);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }

  // Present the rendered image
  SDL_RenderPresent(renderer);
}


// random float
float frand() { return (float)rand() / (float)RAND_MAX; }

// load the lookup table with
// cosine distributed reflection vectors
void initialize_normal_table() {
  srand(time(NULL));
  for (int i = 0; i < TABLE_SIZE; ++i) {
    float theta = 2 * M_PI * frand();
    float rho = sqrtf(-2 * logf(frand()));
    normal_table[i] = rho * cosf(theta);
  }
  table_initialized = 1;
}


// retrieve a random value from the lookup table
float random_normal_distribution() {
  float value = normal_table[table_index];
  table_index = (table_index + 93) % TABLE_SIZE;
  return value;
}

// retrieve a random, normalized vector
Vec3f random_direction() {
  float x = random_normal_distribution();
  float y = random_normal_distribution();
  float z = random_normal_distribution();
  return normalize3f((Vec3f){x, y, z});
}