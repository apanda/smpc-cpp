#include <iostream>
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
  int32_t a_shares[NSHARES];
  int32_t b_shares[NSHARES];
  int32_t c0_shares[NSHARES];
  int32_t c1_shares[NSHARES];
  int32_t c2_shares[NSHARES];
  uint64_t accum = 0;
  for (int i = 0; i < ITERS; i++) {
    uint16_t ar;
    uint16_t br;
    smpc::rng(ar);
    smpc::rng(br);
    ar &= 0x07ff;
    br &= 0x07ff;
    int32_t a = ar;
    int32_t b = br;
    printf("mul %d %d\n", a, b);
    uint64_t start = rdtsc();
    smpc::produceShares(a, a_shares, NSHARES);
    smpc::produceShares(b, b_shares, NSHARES);
    smpc::mulGetShares(a_shares[0], b_shares[0], c0_shares, NSHARES);
    smpc::mulGetShares(a_shares[1], b_shares[1], c1_shares, NSHARES);
    smpc::mulGetShares(a_shares[2], b_shares[2], c2_shares, NSHARES);
    int32_t c0_recomb[NSHARES] = {c0_shares[0], c1_shares[0], c2_shares[0]};
    int32_t c1_recomb[NSHARES] = {c0_shares[1], c1_shares[1], c2_shares[1]};
    int32_t c2_recomb[NSHARES] = {c0_shares[2], c1_shares[2], c2_shares[2]};
    int32_t p_shares[NSHARES];
    p_shares[0] = smpc::mulCombineShares(c0_recomb, NSHARES);
    p_shares[1] = smpc::mulCombineShares(c1_recomb, NSHARES);
    p_shares[2] = smpc::mulCombineShares(c2_recomb, NSHARES);
    int32_t a_recon = smpc::interpolate64(a_shares, NSHARES);
    assert (a == a_recon);
    int32_t b_recon = smpc::interpolate64(b_shares, NSHARES);
    assert (b == b_recon);
    int32_t prod = smpc::interpolate64(p_shares, NSHARES);
    uint64_t end = rdtsc();
    std::cout << "a = " << a 
              << " b = " << b 
              << " prod = " << prod 
              << " time = " << (end - start) 
              << std::endl;
    accum += (end - start);
    assert(prod == a * b);
  }
  std::cout << "Avg time = " << (accum / ITERS) << std::endl;
  return 1;
}
