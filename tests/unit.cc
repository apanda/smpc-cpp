#define CATCH_CONFIG_MAIN
#include "combinations.h"
#include "truth_table.h"
#include "catch.h"
#include "egtopos.h"

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
#define index_for(i, j) ((i * graph.nodes) + j)
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
