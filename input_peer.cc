#include "input_peer.h"
namespace smpc {
  /**
   * Produce shares for a given number
   * number: (In) the number to produce shares for
   * nshares: (In) the number of shares to produce
   * shares: (Out) The returned shares
   */
  void produceShares (int32_t number, int32_t* shares, size_t nshares) {
    assert(nshares >= 3);
    size_t degree = (nshares - 1) / 2;
    int32_t coeffs[degree + 1];
    coeffs[0] = number;
    for (int32_t idx = 1; idx < degree + 1; idx++) {
      int32_t bytes;
      uint16_t rbytes;
      bool ret = smpc::rng(rbytes);
      bytes = rbytes;
      assert(ret);
      coeffs[idx] = bytes % field_size;
    }
    for (int32_t share = 0; share < nshares; share++) {
      int32_t inp = share + 2; //TODO: Sepia says 0 and 1 are bad.
      shares[share] = 0;
      // Evaluate polynomial at point
      uint64_t shareSum = 0;
      for (int32_t deg = 0; deg < degree + 1; deg++) {
        uint64_t val = modPow((uint64_t)inp, deg, field_size);
        uint64_t prod;
        bool overflow = __builtin_umulll_overflow((uint64_t)coeffs[deg], val, &prod);
        assert (!overflow);
        overflow = __builtin_uaddll_overflow(shareSum, (prod % ((uint64_t)field_size)), &shareSum);
        assert (!overflow);
        shareSum = shareSum % (uint64_t)field_size;
      }
      shares[share] = (int32_t) shareSum;
    }
  }

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  int32_t interpolate64 (int32_t* shares, size_t nshares) {
    int64_t fin = 0;
    for (int32_t share = 0; share < nshares; share++) {
      int64_t numerator = 1;
      int64_t denominator = 1;
      for (int32_t oshare = 0; oshare < nshares; oshare++) {
        if (oshare == share) {
          continue;
        }
        numerator *= (-1ll * (int64_t)(oshare + 2));
        denominator *= (share - oshare);
      }
      int64_t lagrange = numerator/denominator;
      int64_t result = lagrange * (int64_t)shares[share];
      fin = (fin + result) % field_size;
    }
    uint64_t finish = (uint64_t)(fin) % field_size;
    const uint64_t mask = 0xffffffff00000000ull;
    assert(!(finish & mask));
    return (int32_t) fin;
  }
};
