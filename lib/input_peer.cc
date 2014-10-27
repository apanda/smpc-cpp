#include "input_peer.h"
namespace smpc {
  /**
   * Produce shares for a given number
   * number: (In) the number to produce shares for
   * nshares: (In) the number of shares to produce
   * shares: (Out) The returned shares
   */
  void produceShares (const mpz_t& number, mpz_t* shares, size_t nshares) {
    assert(nshares >= 3);
    size_t degree = (nshares - 1) / 2;
    mpz_t coeffs[degree + 1];
    mpz_init_set(coeffs[0], number);
    mpz_t field;
    mpz_init_set_si(field, field_size);
    for (int32_t idx = 1; idx < degree + 1; idx++) {
      int32_t bytes;
      uint32_t rbytes;
      bool ret = smpc::rng(rbytes);
      bytes = rbytes;
      assert(ret);
      mpz_init_set_si(coeffs[idx], bytes % field_size);
    }
    mpz_t inp;
    mpz_init(inp);
    for (int32_t share = 0; share < nshares; share++) {
      mpz_set_si(inp, share + 2); //TODO: Sepia says 0 and 1 are bad.
      mpz_set_si(shares[share], 0);
      // Evaluate polynomial at point
      for (int32_t deg = 0; deg < degree + 1; deg++) {
        mpz_t val;
        mpz_init(val);
        mpz_powm_ui(val, inp, deg, field);
        //int64_t prod;
        mpz_t prod;
        mpz_init(prod);
        mpz_mul(prod, val, coeffs[deg]); 
        mpz_mod(prod, prod, field);
        mpz_add(shares[share], shares[share], prod);
        mpz_mod(shares[share], shares[share], field);
      }
    }
    mpz_clear(inp);
    mpz_clear(field);
    for (size_t i = 0; i < degree + 1; i++) {
      mpz_clear(coeffs[i]);
    }
  }

  /**
   * Combine shares to produce the original number.
   * shares: (In) the shares input
   * nshares: (In) the number of shares
   */
  void interpolate64 (mpz_t& fin, const mpz_t* shares, size_t nshares) {
    mpz_t numerator;
    mpz_t denominator;
    mpz_t lagrange; 
    mpz_t field;

    mpz_init_set_si(field, field_size);
    mpz_init_set_si(fin, 0);
    mpz_init(numerator);
    mpz_init(denominator);
    mpz_init(lagrange);
    for (int32_t share = 0; share < nshares; share++) {
      mpz_set_si(numerator, 1);
      mpz_set_si(denominator, 1);
      for (int32_t oshare = 0; oshare < nshares; oshare++) {
        if (oshare == share) {
          continue;
        }
        mpz_mul_si(numerator, numerator, -(oshare + 2));
        mpz_mul_si(denominator, denominator, (share - oshare));
        //numerator *= (-1ll * (int64_t)(oshare + 2));
        //denominator *= (share - oshare);
      }
      mpz_cdiv_q(lagrange, numerator, denominator);
      mpz_addmul(fin, lagrange, shares[share]);
      mpz_mod(fin, fin, field);
      //int64_t lagrange = numerator/denominator;
      //int64_t result = lagrange * (int64_t)shares[share];
      //fin = (fin + result) % field_size;
    }
    mpz_clear(numerator);
    mpz_clear(denominator);
    mpz_clear(lagrange);
    mpz_mod(fin, fin, field);
    mpz_clear(field);
  }
};
