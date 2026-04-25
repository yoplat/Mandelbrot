#include "buddhabrot.h"
#include "mandelbrot.h"
#include "third_party/stb_image_write.h"
#include <algorithm>

#define R_START -2.0
#define R_END 1
#define I_START -1.5
#define I_END 1.5
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
  Complex start{R_START, I_START};
  Complex end{R_END, I_END};
  Mandelbrot mandel(start, end, WIDTH, HEIGHT, MAX_ITER);
  std::vector<uint16_t> mandel_map = mandel.get_map();
  std::vector<uint8_t> rgb(WIDTH * HEIGHT * 3);
  for (size_t i = 0; i < mandel_map.size(); ++i) {
    if (mandel_map[i] == MAX_ITER) {
      rgb[i * 3 + 0] = 0; // inside the set → black
      rgb[i * 3 + 1] = 0;
      rgb[i * 3 + 2] = 0;
    } else {
      const uint8_t *c = MANDELBROT_CLASSIC[mandel_map[i] % 16];
      rgb[i * 3 + 0] = c[0];
      rgb[i * 3 + 1] = c[1];
      rgb[i * 3 + 2] = c[2];
    }
  }
  stbi_write_png("mandelbrot.png", WIDTH, HEIGHT, 3, rgb.data(),
                 3 * WIDTH * sizeof(uint8_t));

  Buddhabrot buddha(Complex{R_START, I_START}, Complex{R_END, I_END}, WIDTH,
                    HEIGHT, WIDTH * HEIGHT * 10, MAX_ITER);
  std::vector<uint32_t> buddha_map = buddha.get_map();
  size_t max_count = *std::max_element(buddha_map.begin(), buddha_map.end());
  std::vector<uint8_t> gray_map(WIDTH * HEIGHT);
  for (size_t i = 0; i < buddha_map.size(); ++i) {
    gray_map[i] = static_cast<uint8_t>((float)buddha_map[i] / max_count * 255);
  }
  stbi_write_png("buddhabrot.png", WIDTH, HEIGHT, 1, gray_map.data(),
                 WIDTH * sizeof(uint8_t));
}
