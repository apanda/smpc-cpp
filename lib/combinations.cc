#include "combinations.h"
namespace smpc {
void combinationProdSum (mpz_t& sum, mpz_t* array, const size_t n, const size_t k) {
  mpz_init(sum);
  size_t indices[k];
  for (size_t i = 0; i < k; i++) {
    indices[i] = i;
  }
  mpz_set_si(sum, 0);
  bool done = false;
  do {
    //T prod = 1;
    mpz_t prod;
    mpz_init_set_si(prod, 1);
    for (size_t i = 0; i < k; i++) {
      mpz_mul(prod, prod, array[indices[i]]);
    }
    mpz_add(sum, sum, prod);
    int32_t ni = 0;
    for (ni = k - 1; ni >= 0 && indices[ni] == (ni + n - k); ni--);
    if (ni == -1) {
      done = true;
    } else {
      indices[ni]++;
      for (size_t i = ni + 1; i < k; i++) {
        indices[i] = indices[i - 1] + 1;
      }
    }
    mpz_clear(prod);
  } while (!done);
}
}
