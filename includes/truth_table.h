#include <cassert>
#include <stdint.h>
#include <cstdio>
#include <gmp.h>
#pragma once
namespace smpc {
  // Expect coeffs to be of size 2 * depth + 1
  void truthTable (uint64_t depth, mpz_t* coeffs, mpz_t& d_coeff);
  void evalTruthTable(mpz_t& result, uint64_t depth, mpz_t* coeffs, const mpz_t& d_coeff, const mpz_t& x);
}
