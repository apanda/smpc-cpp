#include "input_peer.h"
#include "multiplication.h"
namespace {
  // Multiply modulo smpc::field_size 
  uint64_t multMod128(uint64_t a, uint64_t b) {
    __uint128_t prod = 
            (__uint128_t) a *
             (__uint128_t) b;
    __uint128_t mprod = prod % smpc::field_size;
    return (uint64_t) mprod;
  }
  uint64_t multMod(uint64_t a, uint64_t b) {
    uint64_t prod;
    bool overflow; 
    overflow = __builtin_umulll_overflow(a, b, &prod);
    assert(!overflow);
    uint64_t mprod = prod % smpc::field_size;
    return mprod;
  }
}

namespace smpc {
  void mulGetShares (
            uint64_t a,
            uint64_t b,
            uint64_t* shares, 
            size_t nshares) {
    uint64_t prod = multMod(a, b);
    uint64_t prod128 = multMod128(a, b);
    assert(prod == prod128);
    produceShares (prod, shares, nshares);
  }
  uint64_t mulCombineShares (
          uint64_t* shares,
          size_t nshares) {
    return interpolate(shares, nshares); 
  }
}
