#pragma once
#include <cstdint>
#include <vector>

std::vector<uint32_t> buddhabrot_multithreading(double r_start, double r_end,
                                                double i_start, double i_end,
                                                size_t width, size_t height,
                                                size_t max_iter);
