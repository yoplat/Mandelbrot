#include <chrono>
#include <iostream>

struct ScopedTimer {
  const char *label;
  std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
  ~ScopedTimer() {
    std::chrono::duration<double, std::nano> ns =
        std::chrono::steady_clock::now() - t0;
    std::cout << "Time taken for " << label << ": " << ns.count() << "ns"
              << std::endl;
  }
};

class Timer {
public:
  template <typename Duration = std::chrono::microseconds>
  double elapsed() const {
    using FpDuration = std::chrono::duration<double, typename Duration::period>;
    return std::chrono::duration_cast<FpDuration>(
               std::chrono::steady_clock::now() - t0)
        .count();
  }

private:
  std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
};
