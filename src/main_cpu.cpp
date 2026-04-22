#include "mandelbrot.h"
#include "third_party/stb_image_write.h"

#define R_START -2.0
#define R_END 2.0
#define I_START 2.0
#define I_END -2.0
#define WIDTH 800 * 2
#define HEIGHT 800 * 2
#define MAX_ITER 300

const uint8_t MANDELBROT_CLASSIC[16][3] = {
    {66, 30, 15},    {25, 7, 26},     {9, 1, 47},      {4, 4, 73},
    {0, 7, 100},     {12, 44, 138},   {24, 82, 177},   {57, 125, 209},
    {134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
    {255, 170, 0},   {204, 128, 0},   {153, 87, 0},    {106, 52, 3},
};

int main() {
  auto multithreading_map = mandelbrot_points_multithreading(
      R_START, R_END, I_START, I_END, WIDTH, HEIGHT, MAX_ITER);

  std::vector<uint8_t> rgb(WIDTH * HEIGHT * 3);
  for (size_t i = 0; i < multithreading_map.size(); ++i) {
    if (multithreading_map[i] == MAX_ITER) {
      rgb[i * 3 + 0] = 0; // inside the set → black
      rgb[i * 3 + 1] = 0;
      rgb[i * 3 + 2] = 0;
    } else {
      const uint8_t *c = MANDELBROT_CLASSIC[multithreading_map[i] % 16];
      rgb[i * 3 + 0] = c[0];
      rgb[i * 3 + 1] = c[1];
      rgb[i * 3 + 2] = c[2];
    }
  }
  stbi_write_png("mandelbrot.png", WIDTH, HEIGHT, 3, rgb.data(),
                 WIDTH * 3 * sizeof(uint8_t));
}
