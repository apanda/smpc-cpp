#include <cassert>
#include <stdint.h>
#include <cstdio>
#pragma once
namespace smpc {
  template <typename T>
  bool rng(T &value) {
    FILE* f = std::fopen("/dev/urandom", "r");
    if (!f) {
      printf("Open failed\n");
      return false;
    }
    size_t r = fread(&value, sizeof(T), 1, f);
    fclose(f);
    return r == 1;
  }
  int64_t modPow (int64_t num, uint32_t exp, int64_t mod);
}

// Compatibility functions for clang 
#ifndef __clang__
inline bool __builtin_smul_overflow(int32_t a, int32_t b, int32_t* result) {
  *result = a * b;
  return false;
}
inline bool __builtin_smulll_overflow(int64_t a, int64_t b, int64_t* result) {
  *result = a * b;
  return false;
}
inline bool __builtin_sadd_overflow(int32_t a, int32_t b, int32_t* result) {
  *result = a + b;
  return false;
}
inline bool __builtin_saddll_overflow(int64_t a, int64_t b, int64_t* result) {
  *result = a + b;
  return false;
}
inline bool __builtin_ssub_overflow(int32_t a, int32_t b, int32_t* result) {
  *result = a - b;
  return false;
}
#endif
