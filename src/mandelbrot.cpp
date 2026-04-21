#include "mandelbrot.h"

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
