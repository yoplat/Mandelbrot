#include "mandelbrot.h"
#include <thread>

void run_n_rows(std::vector<uint8_t> &map, double r_start, double x_res,
                size_t width, double i_start, double y_res, size_t row_offset,
                size_t n_rows, size_t max_iter) {
  for (size_t py = 0; py < n_rows; ++py) {
    size_t row = row_offset + py;
    double i = i_start + row * y_res; // same formula as iterative
    for (size_t px = 0; px < width; ++px) {
      double r = r_start + px * x_res;
      map[row * width + px] = run_point_double(r, i, max_iter);
    }
  }
}

std::vector<uint8_t>
mandelbrot_points_multithreading(double r_start, double r_end, double i_start,
                                 double i_end, size_t width, size_t height,
                                 size_t max_iter) {
  std::vector<uint8_t> mandelbrot_map(width * height);
  double x_res = (r_end - r_start) / double(width);
  double y_res = (i_end - i_start) / double(height);
  std::vector<std::thread> threads;

  size_t n_chunks = std::thread::hardware_concurrency() * 4;
  size_t rows_per_thread = (height + n_chunks - 1) / n_chunks;
  // ROW MAJOR
  for (size_t py = 0; py < height; py += rows_per_thread) {
    size_t rows = std::min<size_t>(rows_per_thread, height - py);
    threads.emplace_back(run_n_rows, std::ref(mandelbrot_map), r_start, x_res,
                         width, i_start, y_res, py, rows, max_iter);
  }
  for (auto &t : threads)
    t.join();
  return mandelbrot_map;
}

std::vector<uint8_t> mandelbrot_points_iterative(double r_start, double r_end,
                                                 double i_start, double i_end,
                                                 size_t width, size_t height,
                                                 size_t max_iter) {
  std::vector<uint8_t> mandelbrot_map(width * height);
  double x_res = (r_end - r_start) / double(width);
  double y_res = (i_end - i_start) / double(height);

  // ROW MAJOR
  for (size_t py = 0; py < height; ++py) {
    double i = i_start + py * y_res;
    for (size_t px = 0; px < width; ++px) {
      double r = r_start + px * x_res;
      mandelbrot_map[py * width + px] = run_point_double(r, i, max_iter);
    }
  }
  return mandelbrot_map;
}

bool run_point_complex(Complex c, int max_iter) {
  Complex z_n = 0;
  int iter = 0;
  for (; iter < max_iter && square_norm(z_n) < 4; ++iter)
    z_n = z_n * z_n + c;
  return iter == max_iter;
}

bool run_point_double(double r, double i, int max_iter) {
  double r_n = 0;
  double i_n = 0;
  int iter = 0;
  // (a + ib) * (a + ib) = a^2 - b^2 + 2abi
  // z_n = z_n ^ 2 + c
  // (r_n, i_n) = (r_n^2 - i_n^2, 2 * r_n * i_n) + c
  while (iter < max_iter && (r_n * r_n + i_n * i_n) < 4) {
    double r_temp = r_n * r_n - i_n * i_n + r;
    i_n = 2 * r_n * i_n + i;
    r_n = r_temp;
    ++iter;
  }
  return iter == max_iter;
}

void check_run_point() {
  double double_total_time = 0.;
  double complex_total_time = 0.;
  for (double r = -2.0; r < 2.0; r += 0.1) {
    for (double i = -2.0; i < 2.0; i += 0.1) {
      Timer double_timer;
      bool double_result = run_point_double(r, i);
      double_total_time += double_timer.elapsed();
      Timer complex_timer;
      bool complex_result = run_point_complex(Complex(r, i));
      complex_total_time += complex_timer.elapsed();
      if (complex_result != double_result) {
        std::cout << "Failed with: (" << r << "," << i << ")" << std::endl;
      }
    }
  }
  std::cout << "Time taken by Complex: " << complex_total_time << "us"
            << std::endl;
  std::cout << "Time taken by double: " << double_total_time << "us"
            << std::endl;
}

double square_norm(Complex c) {
  return c.real() * c.real() + c.imag() * c.imag();
}
