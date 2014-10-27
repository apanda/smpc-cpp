#include <iostream>
#include <cstdio>
#include <gmp.h>
#include "input_peer.h"
#include "multiplication.h"

static __inline__ uint64_t rdtsc(void)
{
  uint32_t hi, lo;
  __asm__ __volatile__ ("rdtscp" : "=a"(lo), "=d"(hi));
  return ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
}

int main (int argc, char** argv) {
  const int32_t ITERS = 10000;
  const size_t NSHARES = 3;
  mpz_t a, b, a_recon, b_recon, p, p_real;
  mpz_t a_shares[NSHARES];
  mpz_t b_shares[NSHARES];
  mpz_t c0_shares[NSHARES];
  mpz_t c1_shares[NSHARES];
  mpz_t c2_shares[NSHARES];
  mpz_t c0_recomb[NSHARES];
  mpz_t c1_recomb[NSHARES];
  mpz_t c2_recomb[NSHARES];
  mpz_t p_shares[NSHARES];
  mpz_init(a);
  mpz_init(b);
  mpz_init(a_recon);
  mpz_init(b_recon);
  mpz_init(p);
  mpz_init(p_real);
  for (size_t i = 0; i < NSHARES; i++) {
    mpz_init(a_shares[i]);
    mpz_init(b_shares[i]);
    mpz_init(c0_shares[i]);
    mpz_init(c1_shares[i]);
    mpz_init(c2_shares[i]);
    mpz_init(c0_recomb[i]);
    mpz_init(c1_recomb[i]);
    mpz_init(c2_recomb[i]);
    mpz_init(p_shares[i]);
  }
  uint64_t accum = 0;
  for (int i = 0; i < ITERS; i++) {
    uint16_t ar = 5;
    uint16_t br = 7;
    smpc::rng(ar);
    smpc::rng(br);
    //ar &= 0x07ff;
    //br &= 0x07ff;
    //int32_t a = ar;
    //int32_t b = br;
    mpz_init_set_ui(a, ar);
    mpz_init_set_ui(b, br);
    gmp_printf("mul %Zd %Zd\n", a, b);
    uint64_t start = rdtsc();
    smpc::produceShares(a, a_shares, NSHARES);
    smpc::produceShares(b, b_shares, NSHARES);
    smpc::mulGetShares(a_shares[0], b_shares[0], c0_shares, NSHARES);
    smpc::mulGetShares(a_shares[1], b_shares[1], c1_shares, NSHARES);
    smpc::mulGetShares(a_shares[2], b_shares[2], c2_shares, NSHARES);

    //int32_t c0_recomb[NSHARES] = {c0_shares[0], c1_shares[0], c2_shares[0]};
    //int32_t c1_recomb[NSHARES] = {c0_shares[1], c1_shares[1], c2_shares[1]};
    //int32_t c2_recomb[NSHARES] = {c0_shares[2], c1_shares[2], c2_shares[2]};
    mpz_set(c0_recomb[0], c0_shares[0]); mpz_set(c0_recomb[1], c1_shares[0]); mpz_set(c0_recomb[2], c2_shares[0]);
    mpz_set(c1_recomb[0], c0_shares[1]); mpz_set(c1_recomb[1], c1_shares[1]); mpz_set(c1_recomb[2], c2_shares[1]);
    mpz_set(c2_recomb[0], c0_shares[2]); mpz_set(c2_recomb[1], c1_shares[2]); mpz_set(c2_recomb[2], c2_shares[2]);
    //int32_t p_shares[NSHARES];
    smpc::mulCombineShares(p_shares[0], c0_recomb, NSHARES);
    smpc::mulCombineShares(p_shares[1], c1_recomb, NSHARES);
    smpc::mulCombineShares(p_shares[2], c2_recomb, NSHARES);
    smpc::interpolate64(a_recon, a_shares, NSHARES);
    assert(mpz_cmp(a_recon, a) == 0);
    smpc::interpolate64(b_recon, b_shares, NSHARES);
    assert(mpz_cmp(b_recon, b) == 0);
    smpc::interpolate64(p, p_shares, NSHARES);
    uint64_t end = rdtsc();
    //std::cout << "a = " << mpz_get_si(a) 
              //<< " b = " << mpz_get_si(b) 
              //<< " prod = " << mpz_get_si(p) 
              //<< " time = " << (end - start) 
              //<< std::endl;
    mpz_mul(p_real, a, b);
    mpz_mod_ui(p_real, p_real, smpc::field_size);
    gmp_printf("a = %Zd b = %Zd prod = %Zd pr = %Zd\n", a, b, p, p_real);
    accum += (end - start);
    assert(mpz_cmp(p, p_real) == 0);
  }
  std::cout << "Avg time = " << (accum / ITERS) << std::endl;
  return 1;
}
