#include <cassert>
#include <stdint.h>
#include <cstdio>
#include "utils.h"
#pragma once

namespace smpc {
  void mulGetShares (
            int32_t a,
            int32_t b,
            int32_t* shares, 
            size_t nshares);

  int32_t mulCombineShares (
          int32_t* shares,
          size_t nshares);
}
