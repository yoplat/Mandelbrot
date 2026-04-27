#pragma once
#include "mcomplex.h"
#include <cstdint>
#include <vector>

class Buddhabrot {
public:
  Buddhabrot(Complex start, Complex end, size_t width, size_t height,
             size_t n_points, size_t max_iter);

  void run();
  std::vector<uint32_t> &get_map();

private:
  void run_n_points(size_t thread_index);
  void run_single_point(size_t thread_index,
                        std::vector<std::pair<size_t, size_t>> &point_cloud,
                        Complex &c);

  size_t point_to_index(double r, double start, double end, size_t n_points) {
    return (size_t)((r - start) / (end - start) * n_points);
  }

  std::vector<uint32_t> point_count;
  std::vector<std::vector<uint32_t>> thread_maps;

  Complex start, end;
  size_t width, height;
  size_t n_points;
  size_t max_iter;
  size_t points_per_thread;
  size_t n_threads;
};
