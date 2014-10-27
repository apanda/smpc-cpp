#include "unsecproto.h"
#include "truth_table.h"
namespace smpc {
bool unsecurePathComputaton (BGPProblem* problem, 
                                    int32_t dest, 
                              uint32_t* nextHops, 
                                 size_t nhopSize) {
  assert (problem->nodes == nhopSize);
  // Polynomial for checking depth
  mpz_t coeffs[2 * problem->depth + 1];
  mpz_t d_coeff;
  mpz_init(d_coeff);
  for (size_t i = 0; i < (2 * problem->depth) + 1; i++) {
    mpz_init(coeffs[i]);
  }
  truthTable(problem->depth, coeffs, d_coeff);
  
  // Initialize data
  mpz_t finish[nhopSize];
  mpz_t isNull[nhopSize];
  mpz_t delta[nhopSize];
  mpz_t nhops[nhopSize];
  for (size_t i = 0; i < nhopSize; i++) {
    mpz_init_set_si(nhops[i], nhopSize + 1);
    mpz_init_set_si(isNull[i], 1);
    mpz_init_set_si(delta[i], problem->depth);
    mpz_init_set_si(finish[i], 0);
  }
  mpz_set_si(delta[dest], 0);
  mpz_set_si(finish[dest], 1);
  mpz_set_si(isNull[dest], 0);
  mpz_set_si(nhops[dest], dest);
  mpz_t b;
  mpz_t g;
  mpz_t temp1;
  mpz_t temp2;
  //mpz_t temp3;
  //mpz_t temp4;
  mpz_init(b);
  mpz_init(g);
  mpz_init(temp1);
  mpz_init(temp2);
  //mpz_init(temp3);
  //mpz_init(temp4);
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
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_mul_si(temp2, finish[v], problem->customer(u, v));
        mpz_mul(b, temp1, temp2);
        //int32_t b = (1 - finish[u]) * (finish[v]) * (problem->customer(u, v));
        
        mpz_ui_sub(temp1, v, nhops[u]);
        mpz_addmul(nhops[u], b, temp1); 
        //nhops[u] = nhops[u] + b * (v - nhops[u]);
        
        mpz_ui_sub(temp1, 0, isNull[u]);
        mpz_addmul(isNull[u], b, temp1);
        //isNull[u] = isNull[u] + b * (0 - isNull[u]);
        
        mpz_add_ui(temp2, delta[v], 1);
        mpz_sub(temp1, temp2, delta[u]);
        mpz_addmul(delta[u], b, temp1);
        //delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_addmul(finish[u], b, temp1);
        //finish[u] = finish[u] + b * (1 - finish[u]);
      }

      {
        int32_t u = to, v = from;
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_mul_si(temp2, finish[v], problem->customer(u, v));
        mpz_mul(b, temp1, temp2);
        //int32_t b = (1 - finish[u]) * (finish[v]) * (problem->customer(u, v));
        
        mpz_ui_sub(temp1, v, nhops[u]);
        mpz_addmul(nhops[u], b, temp1); 
        //nhops[u] = nhops[u] + b * (v - nhops[u]);
        
        mpz_ui_sub(temp1, 0, isNull[u]);
        mpz_addmul(isNull[u], b, temp1);
        //isNull[u] = isNull[u] + b * (0 - isNull[u]);
        
        mpz_add_ui(temp2, delta[v], 1);
        mpz_sub(temp1, temp2, delta[u]);
        mpz_addmul(delta[u], b, temp1);
        //delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_addmul(finish[u], b, temp1);
        //finish[u] = finish[u] + b * (1 - finish[u]);
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
        mpz_sub_ui(temp2, delta[u], 1);
        mpz_sub(temp1, temp2, delta[v]);
        evalTruthTable(g, problem->depth, coeffs, d_coeff, temp1);
        //int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1));

        mpz_mul_si(temp1, g, problem->peer(u, v));
        mpz_mul(b, finish[v], temp1);
        //int32_t b = finish[v] * problem->peer(u, v) * g;
        
        mpz_ui_sub(temp1, v, nhops[u]);
        mpz_addmul(nhops[u], b, temp1);
        //nhops[u] = nhops[u] + b * (v - nhops[u]);

        mpz_ui_sub(temp1, 0, isNull[u]);
        mpz_addmul(isNull[u], b, temp1);
        //isNull[u] = isNull[u] + b * (0 - isNull[u]);
        
        mpz_add_ui(temp2, delta[v], 1);
        mpz_sub(temp1, temp2, delta[u]);
        mpz_addmul(delta[u], b, temp1);
        //delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_addmul(finish[u], b, temp1);
        //finish[u] = finish[u] + b * (1 - finish[u]);
      }

      {
        int32_t u = to, v = from;
        mpz_sub_ui(temp2, delta[u], 1);
        mpz_sub(temp1, temp2, delta[v]);
        evalTruthTable(g, problem->depth, coeffs, d_coeff, temp1);
        //int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1));

        mpz_mul_si(temp1, g, problem->peer(u, v));
        mpz_mul(b, finish[v], temp1);
        //int32_t b = finish[v] * problem->peer(u, v) * g;
        
        mpz_ui_sub(temp1, v, nhops[u]);
        mpz_addmul(nhops[u], b, temp1);
        //nhops[u] = nhops[u] + b * (v - nhops[u]);

        mpz_ui_sub(temp1, 0, isNull[u]);
        mpz_addmul(isNull[u], b, temp1);
        //isNull[u] = isNull[u] + b * (0 - isNull[u]);
        
        mpz_add_ui(temp2, delta[v], 1);
        mpz_sub(temp1, temp2, delta[u]);
        mpz_addmul(delta[u], b, temp1);
        //delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_addmul(finish[u], b, temp1);
        //finish[u] = finish[u] + b * (1 - finish[u]);
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
        mpz_sub_ui(temp2, delta[u], 1);
        mpz_sub(temp1, temp2, delta[v]);
        evalTruthTable(g, problem->depth, coeffs, d_coeff, temp1);
        //int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1)); 

        mpz_mul_si(temp1, g, problem->provider(u, v));
        mpz_mul(b, finish[v], temp1);
        //int32_t b = finish[v] * problem->provider(u, v) * g;

        mpz_ui_sub(temp1, v, nhops[u]);
        mpz_addmul(nhops[u], b, temp1);
        //nhops[u] = nhops[u] + b * (v - nhops[u]);

        mpz_ui_sub(temp1, 0, isNull[u]);
        mpz_addmul(isNull[u], b, temp1);
        //isNull[u] = isNull[u] + b * (0 - isNull[u]);
        
        mpz_add_ui(temp2, delta[v], 1);
        mpz_sub(temp1, temp2, delta[u]);
        mpz_addmul(delta[u], b, temp1);
        //delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_addmul(finish[u], b, temp1);
        //finish[u] = finish[u] + b * (1 - finish[u]);
      }

      {
        int32_t u = to, v = from;
        mpz_sub_ui(temp2, delta[u], 1);
        mpz_sub(temp1, temp2, delta[v]);
        evalTruthTable(g, problem->depth, coeffs, d_coeff, temp1);
        //int32_t g = (int32_t)evalTruthTable(problem->depth, coeffs, d_coeff, (delta[u] - delta[v] - 1)); 

        mpz_mul_si(temp1, g, problem->provider(u, v));
        mpz_mul(b, finish[v], temp1);
        //int32_t b = finish[v] * problem->provider(u, v) * g;

        mpz_ui_sub(temp1, v, nhops[u]);
        mpz_addmul(nhops[u], b, temp1);
        //nhops[u] = nhops[u] + b * (v - nhops[u]);

        mpz_ui_sub(temp1, 0, isNull[u]);
        mpz_addmul(isNull[u], b, temp1);
        //isNull[u] = isNull[u] + b * (0 - isNull[u]);
        
        mpz_add_ui(temp2, delta[v], 1);
        mpz_sub(temp1, temp2, delta[u]);
        mpz_addmul(delta[u], b, temp1);
        //delta[u] = delta[u] + b * ((delta[v] + 1) - delta[u]);
        
        mpz_ui_sub(temp1, 1, finish[u]);
        mpz_addmul(finish[u], b, temp1);
        //finish[u] = finish[u] + b * (1 - finish[u]);
      }
      IGRAPH_EIT_NEXT(eit);
    }
    igraph_eit_destroy(&eit);
  }
  mpz_clear(b);
  mpz_clear(g);
  mpz_clear(temp1);
  mpz_clear(temp2);
  for (size_t i = 0; i < nhopSize; i++) {
    nextHops[i] = mpz_get_si(nhops[i]);
    mpz_clear(nhops[i]);
    mpz_clear(isNull[i]);
    mpz_clear(delta[i]);
    mpz_clear(finish[i]);
  }
  for (size_t i = 0; i < (2 * problem->depth) + 1; i++) {
    mpz_clear(coeffs[i]);
  }
  return true;
}
}
