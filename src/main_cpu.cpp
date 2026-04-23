#include "buddhabrot.h"
#include "mandelbrot.h"
#include "third_party/stb_image_write.h"
#include <algorithm>

#define R_START -2.0
#define R_END 2.0
#define I_START -2.0
#define I_END 2.0
#define WIDTH 800 * 4
#define HEIGHT 800 * 4
#define MAX_ITER 1000

const uint8_t MANDELBROT_CLASSIC[16][3] = {
    {66, 30, 15},    {25, 7, 26},     {9, 1, 47},      {4, 4, 73},
    {0, 7, 100},     {12, 44, 138},   {24, 82, 177},   {57, 125, 209},
    {134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
    {255, 170, 0},   {204, 128, 0},   {153, 87, 0},    {106, 52, 3},
};

int main() {
  // auto multithreading_map = mandelbrot_points_multithreading(
  //     R_START, R_END, I_START, I_END, WIDTH, HEIGHT, MAX_ITER);
  //
  // std::vector<uint8_t> rgb(WIDTH * HEIGHT * 3);
  // for (size_t i = 0; i < multithreading_map.size(); ++i) {
  //   if (multithreading_map[i] == MAX_ITER) {
  //     rgb[i * 3 + 0] = 0; // inside the set → black
  //     rgb[i * 3 + 1] = 0;
  //     rgb[i * 3 + 2] = 0;
  //   } else {
  //     const uint8_t *c = MANDELBROT_CLASSIC[multithreading_map[i] % 16];
  //     rgb[i * 3 + 0] = c[0];
  //     rgb[i * 3 + 1] = c[1];
  //     rgb[i * 3 + 2] = c[2];
  //   }
  // }
  auto map = buddhabrot_multithreading(R_START, R_END, I_START, I_END, WIDTH,
                                       HEIGHT, MAX_ITER);
  size_t max_count = *std::max_element(map.begin(), map.end());
  std::vector<uint8_t> gray_map(WIDTH * HEIGHT);
  for (size_t i = 0; i < map.size(); ++i) {
    gray_map[i] = static_cast<uint8_t>((float)map[i] / max_count * 255);
  }
  stbi_write_png("mandelbrot.png", WIDTH, HEIGHT, 1, gray_map.data(),
                 WIDTH * sizeof(uint8_t));
}
