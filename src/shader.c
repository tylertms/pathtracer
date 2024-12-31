#include "shader.h"

#include "config.h"
#include "render.h"
#include "scene.h"
#include "vmath.h"

// trace the path of the ray through the scene
// return the accumulated light
Vec3f trace(Ray ray, int x, int y) {
  Vec3f incoming_light = {0, 0, 0};
  Vec3f bg_color = {92.0 / 255, 188.0 / 255, 219.0 / 255};
  float bg_brightness = 0.3;
  Vec3f ray_color = {1.0f, 1.0f, 1.0f};

  for (int i = 0; i <= MAX_BOUNCES; i++) {
    Hit hit = ray_collision(&ray, i, x, y);

    if (hit.did_hit) {
      ray.origin = hit.hit_pos;
      ray.dir = normalize3f(add3f(hit.normal, random_direction()));

      Material mtl = hit.material;
      Vec3f emitted_light = scale3f(mtl.emission_color, mtl.emission_strength);

      incoming_light =
          add3f(incoming_light, mult_color3f(emitted_light, ray_color));
      ray_color = mult_color3f(ray_color, mtl.color);

    } else {
      incoming_light =
          add3f(incoming_light,
                scale3f(mult_color3f(bg_color, ray_color), bg_brightness));
      break;
    }
  }

  return incoming_light;
}

// frag shader (with calculated ray)
// returns color based on pixel position
Vec3f shader(Ray ray, int x, int y) {
  Vec3f total_light = {0, 0, 0};

  for (int i = 0; i < SAMPLES_PER_FRAME; i++) {
    Vec3f result = trace(ray, x, y);
    total_light = add3f(total_light, result);
  }

  total_light = scale3f(total_light, 1.0f / SAMPLES_PER_FRAME);

  return total_light;
}