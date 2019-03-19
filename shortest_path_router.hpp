// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef SHORTEST_PATH_ROUTER_HPP
#define SHORTEST_PATH_ROUTER_HPP

#include <vector>

#include "flow_router.hpp"
#include "tools.hpp"
#include "topology.hpp"

using namespace std;

namespace Network {

class ShortestPathRouter : public FlowRouter {
public:
  enum class TECHNIQUE {
    BY_HOPS, BY_INVERSE_CAPACITY, VIRTUAL_FUNCTION_CALL
  };
  ShortestPathRouter(int K, Topology* topo, TECHNIQUE tech) : 
            FlowRouter(K, topo), tech_(tech) {}
  void PostFlow(Flow flow);
protected:
  TECHNIQUE tech_;
  void ComputeKShortestPaths(Flow* new_flow, const int K, const TECHNIQUE tech);
  double getEdgeCost(Edge* const edge, const TECHNIQUE tech);
  virtual double getEdgeCost(Edge* const edge);
};

} // namespace Network

#endif // SHORTEST_PATH_ROUTER_HPP