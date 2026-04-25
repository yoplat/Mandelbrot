#pragma once

struct Complex {
  double r, i;

  double square_norm() const { return r * r + i * i; }
};
