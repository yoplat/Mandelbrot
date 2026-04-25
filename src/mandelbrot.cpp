#include "mandelbrot.h"
#include <thread>

#define R_START -2.0
#define R_END 2.0
#define I_START 2.0
#define I_END -2.0
#define WIDTH 800 * 2
#define HEIGHT 800 * 2
#define MAX_ITER 300

void Mandelbrot::run() {
  map.resize(width * height, 0);
  double x_res = (end.r - start.r) / double(width);
  double y_res = (end.i - start.i) / double(height);
  size_t n_chunks = std::thread::hardware_concurrency() * 4;
  size_t rows_per_chunk = (height + n_chunks - 1) / n_chunks;
  std::vector<std::thread> threads;
  for (size_t py = 0; py < height; py += rows_per_chunk) {
    size_t rows = std::min<size_t>(rows_per_chunk, height - py);
    threads.emplace_back(&Mandelbrot::run_n_rows, this, x_res, y_res, py, rows);
  }
  for (auto &t : threads)
    t.join();
}

void Mandelbrot::run_n_rows(double x_res, double y_res, size_t row_offset,
                            size_t n_rows) {
  for (size_t py = 0; py < n_rows; ++py) {
    size_t row = row_offset + py;
    double i = start.i + row * y_res;
    for (size_t px = 0; px < width; ++px) {
      double r = start.r + px * x_res;
      map[row * width + px] = run_point({r, i});
    }
  }
}

uint16_t Mandelbrot::run_point(Complex c) const {
  Complex z = {0, 0};
  uint16_t iter = 0;
  while (iter < max_iter && z.square_norm() < 4.0) {
    double r_temp = z.r * z.r - z.i * z.i + c.r;
    z.i = 2 * z.r * z.i + c.i;
    z.r = r_temp;
    ++iter;
  }
  return iter;
}
