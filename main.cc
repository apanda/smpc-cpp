#include <iostream>
#include "input_peer.h"
#include "multiplication.h"
int main (int argc, char** argv) {
  const int32_t ITERS = 10000;
  const size_t NSHARES = 3;
  uint64_t a_shares[NSHARES];
  uint64_t b_shares[NSHARES];
  uint64_t c0_shares[NSHARES];
  uint64_t c1_shares[NSHARES];
  uint64_t c2_shares[NSHARES];
  for (int i = 0; i < ITERS; i++) {
    uint8_t ar;
    uint8_t br;
    smpc::rng(ar);
    smpc::rng(br);
    uint64_t a = 2;
    uint64_t b = 3;
    smpc::produceShares(a, a_shares, NSHARES);
    smpc::produceShares(b, b_shares, NSHARES);
    smpc::mulGetShares(a_shares[0], b_shares[0], c0_shares, NSHARES);
    smpc::mulGetShares(a_shares[1], b_shares[1], c1_shares, NSHARES);
    smpc::mulGetShares(a_shares[2], b_shares[2], c2_shares, NSHARES);
    uint64_t c0_recomb[NSHARES] = {c0_shares[0], c1_shares[0], c2_shares[0]};
    uint64_t c1_recomb[NSHARES] = {c0_shares[1], c1_shares[1], c2_shares[1]};
    uint64_t c2_recomb[NSHARES] = {c0_shares[2], c1_shares[2], c2_shares[2]};
    uint64_t p_shares[NSHARES];
    p_shares[0] = smpc::mulCombineShares(c0_recomb, NSHARES);
    p_shares[1] = smpc::mulCombineShares(c1_recomb, NSHARES);
    p_shares[2] = smpc::mulCombineShares(c2_recomb, NSHARES);
    uint64_t a_recon = smpc::interpolate(a_shares, NSHARES);
    assert (a == a_recon);
    uint64_t b_recon = smpc::interpolate(b_shares, NSHARES);
    assert (b == b_recon);
    uint64_t prod = smpc::interpolate(p_shares, NSHARES);
    std::cout << "a = " << a 
              << " b = " << b 
              << " prod = " << prod << std::endl;
    assert(prod == a * b);
  }
  return 1;
}
