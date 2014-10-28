#include "truth_table.h"
#include "combinations.h"
#include "utils.h"
#include <limits>
//namespace {
  //int64_t ipow(int64_t x, uint64_t y) {
    //int64_t result;
    //for (result = 1; y; y >>= 1) {
      //if (y & 1) {
        //result *= x;
      //}
      //x *= x;
    //}
    //return result;
  //}
//}
namespace smpc {
  // Expect coeffs to be of size 2 * depth + 1
  void truthTable (uint64_t depth, mpz_t* coeffs, mpz_t& d_coeff) {
    mpz_t comb[2*depth];
    for (size_t i = 0; i < (2 * depth); i++) {
      mpz_set_si(coeffs[i], 0);
      mpz_init_set_si(comb[((2 * depth) - 1) - i], -(i - depth));
    }
    mpz_set_si(coeffs[(2*depth)], 0);
    mpz_set_si(d_coeff, 1);
    mpz_t eval;
    mpz_t denom;
    mpz_t temp;
    mpz_t temp2;
    mpz_init(eval);
    mpz_init(denom);
    mpz_init(temp);
    mpz_init(temp2);
    for (int64_t i = 0; i < (2 * depth) && mpz_cmp_si(comb[i], 0) <= 0; i++) {
      mpz_ui_sub(eval, 1, comb[i]);
      mpz_set_si(denom, 1);
      for (int64_t j = 0; j < (2* depth); j++) {
        //denom *= (eval + comb[j]);
        mpz_add(temp, eval, comb[j]);
        mpz_mul(denom, denom, temp);
      }
      //assert(denom != 0);
      assert(mpz_cmp_si(denom, 0) != 0);
      for (int64_t j = 0; j < ((2 * depth) + 1); j++) {
        //coeffs[j] = (denom * coeffs[j]) + 
                    //(d_coeff * combinationProdSum(comb, 2 * depth, j)); 
        combinationProdSum(temp2, comb, 2 * depth, j);
        mpz_mul(temp2, d_coeff, temp2);
        mpz_mul(temp, denom, coeffs[j]);
        mpz_add(coeffs[j], temp, temp2);
      }
      mpz_mul(d_coeff, d_coeff, denom);
      assert(mpz_cmp_si(d_coeff, 0) != 0);
      mpz_sub_ui(comb[i], comb[i], 1);
    }
    mpz_clear(eval);
    mpz_clear(denom);
    mpz_clear(temp);
    mpz_clear(temp2);
    for (size_t i = 0; i < (2*depth); i++) {
      mpz_clear(comb[i]);
    }
  }

  void evalTruthTable(mpz_t& result, uint64_t depth, mpz_t* coeffs, const mpz_t& d_coeff, const mpz_t& x) {
    //int64_t sum = 0;
    assert(mpz_cmp_si(d_coeff, 0) != 0);
    mpz_t temp1;
    mpz_init(temp1);
    mpz_set_si(result, 0);
    for (size_t i = 0; i < ((2 * depth) + 1); i++) {
      uint64_t exp = (2 * depth) - i;
      //int64_t val = ipow(x, exp);
      mpz_pow_ui(temp1, x, exp);
      //sum += (coeffs[i] * val);
      //mpz_mul(temp1, temp1, coeffs[i]);
      mpz_addmul(result, coeffs[i], temp1);
    }
    mpz_clear(temp1);
    mpz_tdiv_q(result, result, d_coeff);
  }
}
