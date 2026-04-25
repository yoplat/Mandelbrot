#include "buddhabrot.h"
#include <random>
#include <thread>

Buddhabrot::Buddhabrot(Complex start, Complex end, size_t width, size_t height,
                       size_t n_points, size_t max_iter)
    : start(start), end(end), width(width), height(height), n_points(n_points),
      max_iter(max_iter) {
  n_threads = std::thread::hardware_concurrency() * 4;
  points_per_thread = (n_points + n_threads - 1) / n_threads;
}

void Buddhabrot::run() {
  point_count.resize(width * height, 0);
  thread_maps.clear();

  for (size_t i = 0; i < n_threads; i++) {
    thread_maps.push_back(std::vector<uint32_t>(width * height));
  }

  std::vector<std::thread> threads;
  for (size_t i = 0; i < n_points; i += points_per_thread) {
    size_t thread_index = i / points_per_thread;
    threads.emplace_back([this, thread_index] { run_n_points(thread_index); });
  }

  size_t index = 0;
  for (auto &t : threads) {
    t.join();
    for (size_t i = 0; i < width * height; ++i) {
      point_count[i] += thread_maps[index][i];
    }
    index++;
  }
}

std::vector<uint32_t> &Buddhabrot::get_map() {
  if (point_count.empty())
    run();
  return point_count;
}

void Buddhabrot::run_n_points(size_t thread_index) {
  std::mt19937 rng;
  std::uniform_real_distribution<double> real_distrib(start.r, end.r);
  std::uniform_real_distribution<double> imag_distrib(start.i, end.i);
  rng.seed(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());

  std::vector<std::pair<size_t, size_t>> point_cloud;
  point_cloud.reserve(max_iter);
  for (size_t i = 0; i < points_per_thread; ++i) {
    Complex c;
    for (;;) {
      c = {real_distrib(rng), imag_distrib(rng)};
      // Checks if point is in main cardioid
      double p = sqrt(((c.r - 1 / 4.0) * (c.r - 1 / 4.0)) + (c.i * c.i));
      double x = p - (2 * p * p) + 1 / 4.0;
      // If inside then skip the cycle for this point
      if (c.r <= x)
        continue;
      // Checks if point is in the second cardioid
      x = (c.r + 1) * (c.r + 1) + c.i * c.i;
      if (x <= 1 / 16.0)
        continue;
      break;
    }
    run_single_point(thread_index, point_cloud, c);
  }
}

void Buddhabrot::run_single_point(
    size_t thread_index, std::vector<std::pair<size_t, size_t>> &point_cloud,
    Complex &c) {
  std::vector<uint32_t> &thread_map = thread_maps[thread_index];
  point_cloud.clear();

  Complex z_n = {0.0, 0.0};
  size_t iter = 0;
  while (iter < max_iter && z_n.square_norm() < 4) {
    double r_temp = z_n.r * z_n.r - z_n.i * z_n.i + c.r;
    z_n.i = 2.0 * z_n.r * z_n.i + c.i;
    z_n.r = r_temp;
    if (z_n.r >= start.r && z_n.r < end.r && z_n.i >= start.i &&
        z_n.i < end.i) {
      size_t r_index = point_to_index(z_n.r, start.r, end.r, width);
      size_t i_index = point_to_index(z_n.i, start.i, end.i, height);
      point_cloud.push_back(std::make_pair(r_index, i_index));
    }
    ++iter;
  }

  if (iter < max_iter) {
    for (auto &p : point_cloud) {
      thread_map[p.second * width + p.first]++;
    }
  }
}
