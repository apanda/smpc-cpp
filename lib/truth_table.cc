#include "truth_table.h"
#include "combinations.h"
#include "utils.h"
#include <limits>
namespace {
  int64_t ipow(int64_t x, uint64_t y) {
    int64_t result;
    for (result = 1; y; y >>= 1) {
      if (y & 1) {
        result *= x;
      }
      x *= x;
    }
    return result;
  }
}
namespace smpc {
  // Expect coeffs to be of size 2 * depth + 1
  void truthTable (uint64_t depth, int64_t* coeffs, int64_t& d_coeff) {
    int64_t comb[2*depth];
    for (size_t i = 0; i < (2 * depth); i++) {
      coeffs[i] = 0;
      comb[((2 * depth) - 1) - i] = -(i - depth);
    }
    coeffs[(2*depth)] = 0;
    d_coeff = 1;
    for (int64_t i = 0; i < (2 * depth) && comb[i] <= 0; i++) {
      int64_t eval = (-comb[i] + 1);
      for (int64_t j = 0; j < 2*depth; j++) {
      }
      int64_t denom = 1;
      for (int64_t j = 0; j < (2* depth); j++) {
        denom *= (eval + comb[j]);
      }
      assert(denom != 0);
      for (int64_t j = 0; j < ((2 * depth) + 1); j++) {
        coeffs[j] = (denom * coeffs[j]) + 
                    (d_coeff * combinationProdSum(comb, 2 * depth, j)); 
      }
      d_coeff *= denom;
      assert(d_coeff != 0);
      comb[i] = comb[i] - 1;
    }
  }

  int64_t evalTruthTable(uint64_t depth, int64_t* coeffs, int64_t d_coeff, int32_t x) {
    int64_t sum = 0;
    assert(d_coeff != 0);
    for (size_t i = 0; i < ((2 * depth) + 1); i++) {
      int64_t exp = (2 * depth) - i;
      int64_t val = ipow(x, exp);
      sum += (coeffs[i] * val);
    }
    return sum / d_coeff;
  }
}
