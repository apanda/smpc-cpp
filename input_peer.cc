#include "input_peer.h"
namespace smpc {
  /**
   * Produce shares for a given number
   * number: (In) the number to produce shares for
   * nshares: (In) the number of shares to produce
   * shares: (Out) The returned shares
   */
  void produceShares (uint64_t number, uint64_t* shares, size_t nshares) {
    assert(nshares >= 3);
    size_t degree = (nshares - 1) / 2;
    uint64_t coeffs[degree + 1];
    coeffs[0] = number;
    for (int32_t idx = 1; idx < degree + 1; idx++) {
      uint64_t bytes;
      bool ret = smpc::rng(bytes);
      assert(ret);
      coeffs[idx] = bytes % field_size;
    }

    for (uint64_t share = 0; share < nshares; share++) {
      uint64_t inp = share + 2; //TODO: Sepia says 0 and 1 are bad.
      shares[share] = 0;
      // Evaluate polynomial at point
      uint64_t shareSum = 0;
      for (uint32_t deg = 0; deg < degree + 1; deg++) {
        uint64_t val = modPow(inp, deg, field_size);
        uint64_t prod;
        bool overflow = __builtin_umulll_overflow(coeffs[deg], val, &prod);
        assert (!overflow);
        overflow = __builtin_uaddll_overflow(shareSum, (prod % field_size), &shareSum);
        assert (!overflow);
        shareSum = shareSum % field_size;
        //__uint128_t prod = ((__uint128_t)coeffs[deg]) * val;
        //shareSum = (shareSum + (prod % field_size)) % field_size;
      }
      shares[share] = (uint64_t) shareSum;
    }
  }

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  uint64_t interpolate (uint64_t* shares, size_t nshares) {
    int64_t fin = 0;
    for (int32_t share = 0; share < nshares; share++) {
      int64_t numerator = 1;
      int64_t denominator = 1;
      bool overflow; 
      for (int32_t oshare = 0; oshare < nshares; oshare++) {
        if (oshare == share) {
          continue;
        }
        int64_t x_point;
        overflow = __builtin_smulll_overflow(-1ull, (oshare + 2), &x_point);
        assert(!overflow);
        overflow = __builtin_smulll_overflow(numerator, x_point, &numerator);
        assert(!overflow);
        int64_t diff;
        overflow = __builtin_ssubll_overflow(share, oshare, &diff);
        assert(!overflow);
        overflow = __builtin_smulll_overflow(denominator, diff, &denominator);
        assert(!overflow);
      }
      int64_t lagrange = numerator/denominator;
      int64_t result;
      overflow = __builtin_smulll_overflow(lagrange, (int64_t)shares[share], &result);
      assert(!overflow);
      overflow = __builtin_saddll_overflow(fin, result, &fin);
      assert(!overflow);
      fin = fin % (int64_t)field_size;
    }
    uint64_t finish = (uint64_t)(fin) % ((int64_t)field_size);
    return (uint64_t) finish;
  }
  int64_t interpolate128 (uint64_t* shares, size_t nshares) {
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
    return (uint64_t) fin;
  }
};
