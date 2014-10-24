#define CATCH_CONFIG_MAIN
#include "combinations.h"
#include "truth_table.h"
#include "catch.h"

TEST_CASE ("Prod Sum Simple Test", "[psum0]" ) {
  int32_t array[] = {-2, -1, 0, 1, 2};
  int32_t array2[] = {0, 1, 2};
  REQUIRE (smpc::combinationProdSum(array, 5, 1) == 0);
  REQUIRE (smpc::combinationProdSum(array, 5, 2) == -5);
  REQUIRE (smpc::combinationProdSum(array, 5, 3) == 0);
  REQUIRE (smpc::combinationProdSum(array, 5, 4) == 4);
  REQUIRE (smpc::combinationProdSum(array, 5, 0) == 1);
  REQUIRE (smpc::combinationProdSum(array2, 3, 0) == 1);
}

TEST_CASE ("Truth table test", "[ttable0]") {
  int64_t coeffs[23];
  int64_t d_coeff;
  smpc::truthTable(1, coeffs, d_coeff);
  REQUIRE(d_coeff > 0);
  REQUIRE(smpc::evalTruthTable(1, coeffs, d_coeff, -1) == 0);
  REQUIRE(smpc::evalTruthTable(1, coeffs, d_coeff, 0) == 0);
  REQUIRE(smpc::evalTruthTable(1, coeffs, d_coeff, 1) == 1);

  smpc::truthTable(2, coeffs, d_coeff);
  REQUIRE(d_coeff != 0);
  REQUIRE(smpc::evalTruthTable(2, coeffs, d_coeff, -2) == 0);
  REQUIRE(smpc::evalTruthTable(2, coeffs, d_coeff, -1) == 0);
  REQUIRE(smpc::evalTruthTable(2, coeffs, d_coeff, 0) == 0);
  REQUIRE(smpc::evalTruthTable(2, coeffs, d_coeff, 1) == 1);
  REQUIRE(smpc::evalTruthTable(2, coeffs, d_coeff, 2) == 1);

  for (int32_t d = 4; d < 7; d++) {
    smpc::truthTable(d, coeffs, d_coeff);
    REQUIRE(d_coeff != 0);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, -4) == 0);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, -2) == 0);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, -1) == 0);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, 0) == 0);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, 1) == 1);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, 2) == 1);
    REQUIRE(smpc::evalTruthTable(d, coeffs, d_coeff, 4) == 1);
  }
}
