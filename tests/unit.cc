#define CATCH_CONFIG_MAIN
#include "combinations.h"
#include "truth_table.h"
#include "catch.h"
#include "egtopos.h"
#include "unsecproto.h"
#include <gmp.h>

TEST_CASE ("Prod Sum Simple Test", "[psum0]" ) {
  //int32_t array[] = {-2, -1, 0, 1, 2};
  //int32_t array2[] = {0, 1, 2};
  mpz_t array[5];
  mpz_t array2[3];
  mpz_t sum;
  mpz_init_set_si(array[0], -2);
  mpz_init_set_si(array[1], -1);
  mpz_init_set_si(array[2], 0);
  mpz_init_set_si(array[3], 1);
  mpz_init_set_si(array[4], 2);

  mpz_init_set_si(array2[0], 0);
  mpz_init_set_si(array2[1], 1);
  mpz_init_set_si(array2[2], 2);
  mpz_init(sum);
  smpc::combinationProdSum(sum, array, 5, 1);
  REQUIRE (mpz_cmp_si(sum, 0) == 0);
  smpc::combinationProdSum(sum, array, 5, 2);
  REQUIRE (mpz_cmp_si(sum, -5) == 0);
  smpc::combinationProdSum(sum, array, 5, 3);
  REQUIRE (mpz_cmp_si(sum, 0) == 0);
  smpc::combinationProdSum(sum, array, 5, 4);
  REQUIRE (mpz_cmp_si(sum, 4) == 0);
  smpc::combinationProdSum(sum, array, 5, 0);
  REQUIRE (mpz_cmp_si(sum, 1) == 0);
  smpc::combinationProdSum(sum, array2, 3, 0);
  REQUIRE (mpz_cmp_si(sum, 1) == 0);
  mpz_clear(sum);
  for (int i = 0; i < 3; i++) {
    mpz_clear(array2[i]);
  }
  for (int i = 0; i < 5; i++) {
    mpz_clear(array[i]);
  }
}

TEST_CASE ("Truth table test", "[ttable0]") {
  mpz_t coeffs[23];
  for (int i = 0; i < 23; i++) {
    mpz_init(coeffs[i]);
  }
  mpz_t d_coeff;
  mpz_init(d_coeff);
  mpz_t result;
  mpz_init(result);
  mpz_t x;
  mpz_init(x);
  smpc::truthTable(1, coeffs, d_coeff);
  REQUIRE(mpz_cmp_si(d_coeff, 0) != 0);

  mpz_set_si(x, -1);
  smpc::evalTruthTable(result, 1, coeffs, d_coeff, x);
  REQUIRE(mpz_cmp_si(result, 0) == 0);

  mpz_set_si(x, 0);
  smpc::evalTruthTable(result, 1, coeffs, d_coeff, x);
  REQUIRE(mpz_cmp_si(result, 0) == 0);

  mpz_set_si(x, 1);
  smpc::evalTruthTable(result, 1, coeffs, d_coeff, x);
  REQUIRE(mpz_cmp_si(result, 1) == 0);

  //gmp_printf("Coeffs are %Zd %Zd %Zd, denom %Zd\n", coeffs[0], coeffs[1], coeffs[2], d_coeff);

  for (int32_t d = 4; d < 11; d++) {
    smpc::truthTable(d, coeffs, d_coeff);
    REQUIRE(mpz_cmp_si(d_coeff, 0) != 0);
    for (int32_t t = -d; t <= 0; t++) {
      mpz_set_si(x, t);
      smpc::evalTruthTable(result, d, coeffs, d_coeff, x);
      REQUIRE(mpz_cmp_si(result, 0) == 0);
    }
    for (int32_t t = 1; t <= d; t++) {
      mpz_set_si(x, t);
      smpc::evalTruthTable(result, d, coeffs, d_coeff, x);
      REQUIRE(mpz_cmp_si(result, 1) == 0);
    }
  }
}

TEST_CASE ("Graph check", "[graph]") {
  smpc::BGPProblem graph;
  smpc::example::exampleTopo1(&graph);
  igraph_vs_t vs;
  igraph_vs_nonadj(&vs, 0, IGRAPH_ALL);
  igraph_integer_t size;
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 4);

  igraph_vs_nonadj(&vs, 1, IGRAPH_ALL);
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 4);

  igraph_vs_nonadj(&vs, 2, IGRAPH_ALL);
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 4);

  igraph_vs_nonadj(&vs, 3, IGRAPH_ALL);
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 5);

  igraph_vs_nonadj(&vs, 4, IGRAPH_ALL);
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 6);

  igraph_vs_nonadj(&vs, 5, IGRAPH_ALL);
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 5);

  igraph_vs_nonadj(&vs, 6, IGRAPH_ALL);
  igraph_vs_size(&graph.graph, &vs, &size);
  REQUIRE(size == 5);
  for (int i = 0; i < graph.nodes; i++) {
    for (int j = 0; j < graph.nodes; j++) {
      bool provider = (graph.provider(i, j) == 1) &&
                      (graph.customer(j, i) == 1);
      bool nprovider = (graph.provider(i, j) == 0) &&
                      (graph.customer(j, i) == 0);
      bool correct = provider || nprovider;
      REQUIRE(correct);
      
      bool peer = (graph.peer(i, j) == 1) &&
                  (graph.peer(j, i) == 1);
      bool npeer = (graph.peer(i, j) == 0) &&
                   (graph.peer(j, i) == 0);
      correct = peer || npeer;
      REQUIRE(correct);
    }
  }
}

TEST_CASE ("Unsec BGP check", "[ubgp]") {
  smpc::BGPProblem graph;
  smpc::example::exampleTopo1(&graph);
  uint32_t nhop[graph.nodes];
  REQUIRE(unsecurePathComputaton(&graph, 0, nhop, graph.nodes));
  REQUIRE(nhop[0] == 0);
  REQUIRE(nhop[1] == 0);
  REQUIRE(nhop[2] == 0);
  REQUIRE(nhop[3] == 2);
  REQUIRE(nhop[4] == 2);
  REQUIRE(nhop[5] == 1);
  REQUIRE(nhop[6] == 0);
  REQUIRE(unsecurePathComputaton(&graph, 1, nhop, graph.nodes));
  REQUIRE(nhop[0] == 1);
  REQUIRE(nhop[1] == 1);
  REQUIRE(nhop[2] == 0);
  REQUIRE(nhop[3] == 2);
  REQUIRE(nhop[4] == 2);
  REQUIRE(nhop[5] == 1);
  REQUIRE(nhop[6] == 1);
  REQUIRE(unsecurePathComputaton(&graph, 2, nhop, graph.nodes));
  REQUIRE(nhop[0] == 2);
  REQUIRE(nhop[1] == 0);
  REQUIRE(nhop[2] == 2);
  REQUIRE(nhop[3] == 2);
  REQUIRE(nhop[4] == 2);
  REQUIRE(nhop[5] == 1);
  REQUIRE(nhop[6] == 0);
  REQUIRE(unsecurePathComputaton(&graph, 3, nhop, graph.nodes));
  REQUIRE(nhop[0] == 2);
  REQUIRE(nhop[1] == 0);
  REQUIRE(nhop[2] == 3);
  REQUIRE(nhop[3] == 3);
  REQUIRE(nhop[4] == 2);
  REQUIRE(nhop[5] == 3);
  REQUIRE(nhop[6] == 0);
  REQUIRE(unsecurePathComputaton(&graph, 5, nhop, graph.nodes));
  REQUIRE(nhop[0] == 1);
  REQUIRE(nhop[1] == 5);
  REQUIRE(nhop[2] == 0);
  REQUIRE(nhop[3] == 5);
  REQUIRE(nhop[4] == 2);
  REQUIRE(nhop[5] == 5);
  REQUIRE(nhop[6] == 1);
}
