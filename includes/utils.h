#include <cassert>
#include <stdint.h>
#include <cstdio>
#pragma once
namespace smpc {
  template <typename T>
  bool rng(T &value) {
    FILE* f = std::fopen("/dev/random", "r");
    if (!f) {
      printf("Open failed\n");
      return false;
    }
    size_t r = fread(&value, sizeof(T), 1, f);
    fclose(f);
    return r == 1;
  }
  uint64_t modPow (uint64_t num, uint32_t exp, uint64_t mod);
}
