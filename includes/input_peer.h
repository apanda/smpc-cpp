#include <cassert>
#include <stdint.h>
#include <cstdio>
#include "utils.h"
#pragma once

namespace smpc {
  //const int32_t field_size = 571710324769;
  const int32_t field_size = 2147483647;
  /**
   * Produce shares for a given number
   * number: (In) the number to produce shares for
   * nshares: (In) the number of shares to produce
   * shares: (Out) The returned shares
   */
  void produceShares (int32_t number, int32_t* shares, size_t nshares);

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  int32_t interpolate (int32_t* shares, size_t nshares);

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  int32_t interpolate64 (int32_t* shares, size_t nshares);
};
