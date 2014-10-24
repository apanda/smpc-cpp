#include "egtopos.h"
namespace smpc{
namespace example {

void exampleTopo1 (BGPProblem* graph) {  
  const igraph_real_t edges[] = {0, 1,
                     0, 2,
                     0, 6,
                     1, 6,
                     1, 5,
                     2, 3,
                     2, 4,
                     3, 5
  };
  const size_t LENGTH = 16;
  igraph_vector_t vedges;
  igraph_vector_view(&vedges, edges, LENGTH);
  int ret = igraph_create(&graph->graph, &vedges, 7, false);
  assert(!ret);

  static int32_t provider[] = {
      0, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0,
      0, 1, 0, 0, 0, 0, 0,
      1, 1, 0, 0, 0, 0, 0
  };

  static int32_t customer[] = {
      0, 1, 1, 0, 0, 0, 1,
      0, 0, 0, 0, 0, 1, 1,
      0, 0, 0, 1, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0
  };

  static int32_t peer[] = {
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0
  };
  graph->_provider = provider;
  graph->_customer = customer;
  graph->_peer = peer;
  graph->depth = 4;
  graph->nodes = 7;
}

}
}
