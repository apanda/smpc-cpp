#include <igraph/igraph.h>
#include <cassert>
#pragma once
namespace smpc{
namespace example {
  struct BGPProblem {
    igraph_t graph;
    int32_t* _provider;
    int32_t* _customer;
    int32_t* _peer;
    int32_t depth;
    int32_t nodes;
    inline const int32_t provider(int32_t i, int32_t j) const {
      return _provider[(i * nodes) + j];
    }

    inline const int32_t customer(int32_t i, int32_t j) const {
      return _customer[(i * nodes) + j];
    }

    inline const int32_t peer(int32_t i, int32_t j) const {
      return _peer[(i * nodes) + j];
    }
  };
  void exampleTopo1 (BGPProblem* example);
}
}
