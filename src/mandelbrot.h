#include "mcomplex.h"
#include <vector>

class Mandelbrot {
public:
  Mandelbrot(Complex start, Complex end, size_t width, size_t height,
             size_t max_iter)
      : map(0), start(start), end(end), width(width), height(height),
        max_iter(max_iter) {}

  void run();

  std::vector<uint16_t> &get_map() {
    if (map.empty())
      run();
    return map;
  }

private:
  void run_n_rows(double x_res, double y_res, size_t row_offset, size_t n_rows);

  uint16_t run_point(Complex c) const;

  std::vector<uint16_t> map;
  Complex start, end;
  size_t width, height;
  size_t max_iter;
};
