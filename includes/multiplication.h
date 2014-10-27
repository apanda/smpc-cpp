#include <cassert>
#include <stdint.h>
#include <cstdio>
#include <gmp.h>
#include "utils.h"
#pragma once

namespace smpc {
  void mulGetShares (
            const mpz_t& a,
            const mpz_t& b,
            mpz_t* shares, 
            size_t nshares);

  void mulCombineShares (
          mpz_t& result,
          const mpz_t* shares,
          size_t nshares);
}
