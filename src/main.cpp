#include "mandelbrot.h"

#define R_START -2.0
#define R_END 2.0
#define I_START 2.0
#define I_END -2.0
#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 100

int main() {
  Timer t_iter;
  auto iterative_map = mandelbrot_points_iterative(
      R_START, R_END, I_START, I_END, WIDTH, HEIGHT, MAX_ITER);
  std::cout << "Time taken for iterative: " << t_iter.elapsed() << std::endl;
  Timer t_multi;
  auto multithreading_map = mandelbrot_points_multithreading(
      R_START, R_END, I_START, I_END, WIDTH, HEIGHT, MAX_ITER);
  std::cout << "Time taken for multithreading: " << t_multi.elapsed()
            << std::endl;
  int mismatches = 0;
  for (size_t i = 0; i < iterative_map.size(); ++i) {
    if (iterative_map[i] != multithreading_map[i]) {
      if (mismatches < 20) {
        std::cout << "Error at index " << i
                  << ": iter=" << int(iterative_map[i])
                  << " multi=" << int(multithreading_map[i]) << "\n";
      }
      ++mismatches;
    }
  }
  std::cout << "Total mismatches: " << mismatches << "/" << iterative_map.size()
            << "\n";
  return 0;
}
