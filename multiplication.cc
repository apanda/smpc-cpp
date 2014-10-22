#include "input_peer.h"
#include "multiplication.h"
namespace {
  // Multiply modulo smpc::field_size 
  int32_t multMod64(int32_t a, int32_t b) {
    int64_t prod = 
            (int64_t) a *
             (int64_t) b;
    int64_t mprod = prod % (uint64_t) smpc::field_size;
    uint64_t mask = 0xffffffff00000000ull;
    assert(!(mprod & mask));
    return (int32_t) mprod;
  }
}

namespace smpc {
  void mulGetShares (
            int32_t a,
            int32_t b,
            int32_t* shares, 
            size_t nshares) {
    int32_t prod64 = multMod64(a, b);
    produceShares (prod64, shares, nshares);
  }
  int32_t mulCombineShares (
          int32_t* shares,
          size_t nshares) {
    int32_t comb64 = interpolate64(shares, nshares);
    return comb64;
  }
}
