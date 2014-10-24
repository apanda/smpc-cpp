#include <cassert>
#include <stdint.h>
#include <cstdio>
#pragma once
namespace smpc {
  // Expect coeffs to be of size 2 * depth + 1
  void truthTable (uint64_t depth, int64_t* coeffs, int64_t& d_coeff);
  int64_t evalTruthTable(uint64_t depth, int64_t* coeffs, int64_t d_coeff, int32_t x);
}
