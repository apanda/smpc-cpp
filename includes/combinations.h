#include <cassert>
#include <stdint.h>
#include <cstdio>
#include <gmp.h>
#pragma once
namespace smpc {
  // Useful for building interpolated polynomial representing the depth
  // truth table
  void combinationProdSum (mpz_t& sum, mpz_t* array, const size_t n, const size_t k);
}
