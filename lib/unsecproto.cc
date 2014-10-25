#include "unsecproto.h"
#include "truth_table.h"
namespace smpc {
bool unsecurePathComputaton (BGPProblem* problem, 
                                    int32_t dest, 
                              uint32_t* nextHops, 
                                 size_t nhopSize) {
  assert (problem->nodes == nhopSize);
  int32_t finish[nhopSize];
  int32_t isNull[nhopSize];
  int32_t delta[nhopSize];
  // Polynomial for checking depth
  int64_t coeffs[2 * problem->depth + 1];
  int64_t d_coeff;
  truthTable(problem->depth, coeffs, d_coeff);
  
  // Initialize data
  for (size_t i = 0; i < nhopSize; i++) {
    nextHops[i] = nhopSize + 1;
    isNull[i] = 1;
    delta[i] = problem->depth;
    finish[i] = 0;
  }
  delta[dest] = 0;
  finish[dest] = 1;
  isNull[dest] = 0;
  nextHops[dest] = dest;

  // Repeat depth times (BFS of customers)
  for (int i = 0; i < problem->depth; i++) {
    igraph_eit_t eit;
    int ret = igraph_eit_create(&problem->graph, igraph_ess_all(IGRAPH_EDGEORDER_FROM), &eit);
    assert(!ret);
    while (!IGRAPH_EIT_END(eit)) {
      igraph_integer_t from, to;
      igraph_edge(&problem->graph, IGRAPH_EIT_GET(eit), &from, &to);
      // Consider both from -> to and to -> from
      {
        int32_t u = from, v = to;
        int32_t b = (1 - finish[u]) * (finish[v]) * (problem->customer(u, v));
        nextHops[u] = nextHops[u] + b * (v - nextHops[u]);
        isNull[u] = isNull[u] + b * (0 - isNull[u]);
        delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        finish[u] = finish[u] + b * (1 - finish[u]);
      }

      {
        int32_t u = to, v = from;
        int32_t b = (1 - finish[u]) * (finish[v]) * (problem->customer(u, v));
        nextHops[u] = nextHops[u] + b * (v - nextHops[u]);
        isNull[u] = isNull[u] + b * (0 - isNull[u]);
        delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        finish[u] = finish[u] + b * (1 - finish[u]);
      }
      IGRAPH_EIT_NEXT(eit);
    }
    igraph_eit_destroy(&eit);
  }
  // Repeat once (BFS for peers)
  do {
    igraph_eit_t eit;
    int ret = igraph_eit_create(&problem->graph, igraph_ess_all(IGRAPH_EDGEORDER_FROM), &eit);
    assert(!ret);
    while (!IGRAPH_EIT_END(eit)) {
      igraph_integer_t from, to;
      igraph_edge(&problem->graph, IGRAPH_EIT_GET(eit), &from, &to);
      // Consider both from -> to and to -> from
      {
        int32_t u = from, v = to;
        int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1));
        int32_t b = finish[v] * problem->peer(u, v) * g;
        nextHops[u] = nextHops[u] + b * (v - nextHops[u]);
        delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        isNull[u] = isNull[u] + b * (0 - isNull[u]);
        finish[u] = finish[u] + b * (1 - finish[u]);
      }

      {
        int32_t u = to, v = from;
        int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1));
        int32_t b = finish[v] * problem->peer(u, v) * g;
        nextHops[u] = nextHops[u] + b * (v - nextHops[u]);
        delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        isNull[u] = isNull[u] + b * (1 - isNull[u]);
        finish[u] = finish[u] + b * (1 - finish[u]);
      }
      IGRAPH_EIT_NEXT(eit);
    }
    igraph_eit_destroy(&eit);
  } while (false);
  // Repeat depth times (BFS of provider)
  for (int i = 0; i < problem->depth; i++) {
    igraph_eit_t eit;
    int ret = igraph_eit_create(&problem->graph, igraph_ess_all(IGRAPH_EDGEORDER_FROM), &eit);
    assert(!ret);
    while (!IGRAPH_EIT_END(eit)) {
      igraph_integer_t from, to;
      igraph_edge(&problem->graph, IGRAPH_EIT_GET(eit), &from, &to);
      // Consider both from -> to and to -> from
      {
        int32_t u = from, v = to;
        int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1)); 
        int32_t b = finish[v] * problem->provider(u, v) * g;
        nextHops[u] = nextHops[u] + b * (v - nextHops[u]);
        delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        isNull[u] = isNull[u] + b * (0 - isNull[u]);
        finish[u] = finish[u] + b * (1 - finish[u]);
      }

      {
        int32_t u = to, v = from;
        int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1)); 
        int32_t b = finish[v] * problem->provider(u, v) * g;
        nextHops[u] = nextHops[u] + b * (v - nextHops[u]);
        delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        isNull[u] = isNull[u] + b * (0 - isNull[u]);
        finish[u] = finish[u] + b * (1 - finish[u]);
      }
      IGRAPH_EIT_NEXT(eit);
    }
    igraph_eit_destroy(&eit);
  }
  return true;
}
}
