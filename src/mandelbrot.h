#include <complex>
#include <cstdint>
#include <vector>

#include "timer.h"

typedef std::complex<double> Complex;

/*
 * Computes the square norm of a complex number c.
 */
double square_norm(Complex c);

/*
 * Check if a point is inside the mandelbrot set.
 * This is done by iterating with the expression `z_n` = `z_n^2` + `c`, with
 * z_n starting from 0 and c being the input point coordinates. If `z_n` doesn't
 * diverge after `max_iter` then it's inside.
 * Uses the c++ Complex class.
 */
uint16_t run_point_complex(Complex c, int max_iter = 100);

/*
 * Check if a point is inside the mandelbrot set.
 * This is done by iterating with the expression `z_n` = `z_n^2` + `c`, with
 * z_n starting from 0 and c being the input point coordinates. If `z_n` doesn't
 * diverge after `max_iter` then it's inside.
 * Uses simple double(s).
 */
uint16_t run_point_double(double r, double i, int max_iter = 100);

/*
 * Returns a vector representing a grid of points to check if they are
 * contained in the mandelbrot set.
 * Each element is true iff it's inside the set.
 * `width` and `height` are the resolutions in the real and img axes;
 * Single threaded solution.
 */
std::vector<uint16_t> mandelbrot_points_iterative(double r_start, double r_end,
                                                  double i_start, double i_end,
                                                  size_t width, size_t height,
                                                  size_t max_iter = 100);
/*
 * Returns a vector representing a grid of points to check if they are
 * contained in the mandelbrot set.
 * Each element is true iff it's inside the set.
 * `width` and `height` are the resolutions in the real and img axes;
 * Single threaded solution.
 */
std::vector<uint16_t>
mandelbrot_points_multithreading(double r_start, double r_end, double i_start,
                                 double i_end, size_t width, size_t height,
                                 size_t max_iter = 100);

/*
 * Checks that both run_point_complex and run_point_double are working
 * as inteded and tests performance.
 */
void check_run_point();

/*
 * Checks that iterative and multithreading give same results and compares
 * the performance.
 */
void check_iterative_vs_multithreading();
