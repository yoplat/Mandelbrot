#include <complex>

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
bool run_point_complex(Complex c, int max_iter = 100);

/*
 * Check if a point is inside the mandelbrot set.
 * This is done by iterating with the expression `z_n` = `z_n^2` + `c`, with
 * z_n starting from 0 and c being the input point coordinates. If `z_n` doesn't
 * diverge after `max_iter` then it's inside.
 * Uses simple double(s).
 */
bool run_point_double(double r, double i, int max_iter = 100);

/*
 * Checks that both run_point_complex and run_point_double are working
 * as inteded and tests performance.
 */
void check_run_point();
