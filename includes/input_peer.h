#include <cassert>
#include <stdint.h>
#include <cstdio>
#include "utils.h"
#pragma once

namespace smpc {
  //const uint64_t field_size = 571710324769;
  const uint64_t field_size = 2147483647ull;
  /**
   * Produce shares for a given number
   * number: (In) the number to produce shares for
   * nshares: (In) the number of shares to produce
   * shares: (Out) The returned shares
   */
  void produceShares (uint64_t number, uint64_t* shares, size_t nshares);

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  uint64_t interpolate (uint64_t* shares, size_t nshares);

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  int64_t interpolate128 (uint64_t* shares, size_t nshares);
};
