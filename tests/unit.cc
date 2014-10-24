#define CATCH_CONFIG_MAIN
#include "combinations.h"
#include "catch.h"

TEST_CASE ("Prod Sum Simple Test", "[psum0]" ) {
  int32_t array[] = {-2, -1, 0, 1, 2};
  REQUIRE (smpc::combinationProdSum(array, 5, 1) == 0);
  REQUIRE (smpc::combinationProdSum(array, 5, 2) == -5);
  REQUIRE (smpc::combinationProdSum(array, 5, 3) == 0);
  REQUIRE (smpc::combinationProdSum(array, 5, 4) == 4);

}
