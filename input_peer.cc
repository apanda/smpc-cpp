#include <iostream>
#include <cassert>
#include <stdint.h>
#include <cstdio>

namespace {
  bool rng64(uint64_t &value) {
    FILE* f = std::fopen("/dev/random", "r");
    if (!f) {
      printf("Open failed\n");
      return false;
    }
    size_t r = fread(&value, sizeof(uint64_t), 1, f);
    fclose(f);
    return r == 1;
  }

  uint64_t modPow (uint64_t num, uint32_t exp, uint64_t mod) {
    __uint128_t result = 1;
    __uint128_t n = num;
    for (result = 1; exp; exp >>= 1) {
      if (exp & 1) {
        result = ((result % mod) * (n % mod)) % mod;
      }
      n = ((n % mod) * (num % mod)) % mod;
    }
    return (uint64_t)result;
  }
}

namespace smpc {
  const uint64_t field_size = 571710324769;
  /**
   * Produce shares for a given number
   * number: (In) the number to produce shares for
   * nshares: (In) the number of shares to produce
   * shares: (Out) The returned shares
   */
  void produceShares (uint64_t number, size_t nshares, uint64_t* shares) {
    assert(nshares >= 3);
    size_t degree = (nshares - 1) / 2;
    uint64_t coeffs[degree + 1];
    coeffs[0] = number;
    for (int32_t idx = 1; idx < degree + 1; idx++) {
      uint64_t bytes;
      bool ret = rng64(bytes);
      assert(ret);
      coeffs[idx] = bytes % field_size;
    }

    for (uint64_t share = 0; share < nshares; share++) {
      uint64_t inp = share + 2; //TODO: Sepia says 0 and 1 are bad.
      shares[share] = 0;
      // Evaluate polynomial at point
      __uint128_t shareSum = 0;
      for (uint32_t deg = 0; deg < degree + 1; deg++) {
        uint64_t val = modPow(inp, deg, field_size);
        __uint128_t prod = ((__uint128_t)coeffs[deg]) * val;
        shareSum = (shareSum + (prod % field_size)) % field_size;
      }
      shares[share] = (uint64_t) shareSum;
    }
  }

  int64_t interpolate (uint64_t* shares, size_t nshares) {
    __int128_t fin = 0;
    for (int32_t share = 0; share < nshares; share++) {
      int64_t numerator = 1;
      int64_t denominator = 1;
      for (int32_t oshare = 0; oshare < nshares; oshare++) {
        if (oshare == share) {
          continue;
        }
        numerator *= (-1ull * (oshare + 2));
        denominator *= (share - oshare);
      }
      int64_t lagrange = numerator/denominator;
      int64_t result = lagrange * (int64_t)shares[share];
      fin = (fin + result) % field_size;
    }
    uint64_t finish = (uint64_t)(fin) % field_size;
    return (int64_t) fin;
  }
};

int main (int argc, char** argv) {
  uint64_t shares[3];
  const size_t NSHARES = 3;
  const int32_t ITERS = 10000;
  for (int i = 0; i < ITERS; i++) {
    uint64_t num = i;
    smpc::produceShares(num, NSHARES, shares);
    for (int i = 0; i < 3; i++) {
      std::cout << "Share " << i << "  " << shares[i] << std::endl;
    }
    uint64_t interp =  smpc::interpolate(shares, 3);
    std::cout << "Interpolated " << interp << std::endl;
    assert(interp == num);
  }
  return 1;
}
