#include <cassert>
#include <stdint.h>
#include <cstdio>
#include "utils.h"
#pragma once

namespace smpc {
  void mulGetShares (
            uint64_t a,
            uint64_t b,
            uint64_t* shares, 
            size_t nshares);

  uint64_t mulCombineShares (
          uint64_t* shares,
          size_t nshares);
}
