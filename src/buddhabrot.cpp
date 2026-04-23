#include "buddhabrot.h"
#include <thread>

#define R_START -2.0
#define R_END 2.0
#define I_START 2.0
#define I_END -2.0
#define WIDTH 800 * 2
#define HEIGHT 800 * 2
#define MAX_ITER 300

size_t point_to_int(double r, double start, double end, size_t n_points) {
  int idx = (int)((r - start) / (end - start) * n_points);
  if (idx < 0)
    idx = 0;
  if (idx >= (int)n_points)
    idx = n_points - 1;
  return (size_t)idx;
}

void run_single_point(std::vector<uint32_t> &total_point_count,
                      std::vector<std::pair<size_t, size_t>> &point_cloud,
                      double r, double i, double r_start, double r_end,
                      double i_start, double i_end, size_t width, size_t height,
                      size_t max_iter) {
  point_cloud.clear();
  double r_n = 0;
  double i_n = 0;
  size_t iter = 0;
  while (iter < max_iter && (r_n * r_n + i_n * i_n) < 4) {
    double r_temp = r_n * r_n - i_n * i_n + r;
    i_n = 2 * r_n * i_n + i;
    r_n = r_temp;
    if (r_n >= r_start && r_n < r_end && i_n >= i_start && i_n < i_end) {
      size_t r_index = point_to_int(r_n, r_start, r_end, width);
      size_t i_index = point_to_int(i_n, i_start, i_end, height);
      point_cloud.push_back(std::make_pair(r_index, i_index));
    }
    ++iter;
  }
  if (iter < max_iter) {
    for (auto &p : point_cloud) {
      total_point_count[p.second * width + p.first]++;
    }
  }
}

void run_n_rows(std::vector<uint32_t> &total_point_count, double r_start,
                double r_end, double i_start, double i_end, size_t width,
                size_t height, size_t row_offset, size_t n_rows,
                size_t max_iter) {
  std::vector<std::pair<size_t, size_t>> point_cloud;
  point_cloud.reserve(max_iter);
  double x_res = (r_end - r_start) / double(width);
  double y_res = (i_end - i_start) / double(height);
  for (size_t py = 0; py < n_rows; ++py) {
    size_t row = row_offset + py;
    double i = i_start + row * y_res;
    for (size_t px = 0; px < width; ++px) {
      double r = r_start + px * x_res;
      run_single_point(total_point_count, point_cloud, r, i, r_start, r_end,
                       i_start, i_end, width, height, max_iter);
    }
  }
}

std::vector<uint32_t> buddhabrot_multithreading(double r_start, double r_end,
                                                double i_start, double i_end,
                                                size_t width, size_t height,
                                                size_t max_iter) {
  std::vector<uint32_t> buddhabrot_map(width * height);
  // On my machine with 16 threads this is equal to 64
  size_t n_chunks = std::thread::hardware_concurrency() * 4;
  size_t rows_per_thread = (height + n_chunks - 1) / n_chunks;

  std::vector<std::thread> threads;
  std::vector<std::vector<uint32_t>> threads_maps;
  for (size_t i = 0; i < n_chunks; i++) {
    threads_maps.push_back(std::vector<uint32_t>(width * height));
  }

  // ROW MAJOR
  for (size_t py = 0; py < height; py += rows_per_thread) {
    size_t rows = std::min<size_t>(rows_per_thread, height - py);
    size_t chunk_idx = py / rows_per_thread;
    threads.emplace_back(run_n_rows, std::ref(threads_maps[chunk_idx]), r_start,
                         r_end, i_start, i_end, width, height, py, rows,
                         max_iter);
  }
  size_t index = 0;
  for (auto &t : threads) {
    t.join();
    for (size_t i = 0; i < width * height; ++i) {
      buddhabrot_map[i] += threads_maps[index][i];
    }
    index++;
  }
  return buddhabrot_map;
}
