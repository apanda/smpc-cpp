#include "input_peer.h"
#include "multiplication.h"
//namespace {
  //// Multiply modulo smpc::field_size 
  //int32_t multMod64(int32_t a, int32_t b) {
    //int64_t prod = 
            //(int64_t) a *
             //(int64_t) b;
    //int64_t mprod = prod % (uint64_t) smpc::field_size;
    //return (int32_t) mprod;
  //}
//}

namespace smpc {
  void mulGetShares (
            const mpz_t& a,
            const mpz_t& b,
            mpz_t* shares, 
            size_t nshares) {
    //int32_t prod64 = multMod64(a, b);
    mpz_t prod64;
    mpz_init(prod64);
    mpz_mul(prod64, a, b);
    mpz_mod_ui(prod64, prod64, field_size);
    produceShares (prod64, shares, nshares);
    mpz_clear(prod64);
  }
  void mulCombineShares (
          mpz_t& result,
          const mpz_t* shares,
          size_t nshares) {
    interpolate64(result, shares, nshares);
  }
}
