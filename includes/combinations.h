#include <cassert>
#include <stdint.h>
#include <cstdio>
#pragma once
namespace smpc {
  // Useful for building interpolated polynomial representing the depth
  // truth table
  template <typename T>
  T combinationProdSum (const T* array, const size_t n, const size_t k) {
    T sum = 0;
    T indices[k];
    for (size_t i = 0; i < k; i++) {
      indices[i] = i;
    }
    bool done = false;
    do {
      T prod = 1;
      for (size_t i = 0; i < k; i++) {
        prod *= array[indices[i]];
      }
      sum += prod;
      int32_t ni = 0;
      for (ni = k - 1; ni >= 0 && indices[ni] == (ni + n - k); ni--);
      if (ni == -1) {
        done = true;
      } else {
        indices[ni]++;
        for (size_t i = ni + 1; i < k; i++) {
          indices[i] = indices[i - 1] + 1;
        }
      }
    } while (!done);
    return sum;
  }
}
