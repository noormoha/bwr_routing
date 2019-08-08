// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <limits>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

#include "shortest_path_router.hpp"

using namespace std;

namespace Network {

void ShortestPathRouter::ComputeShortestPath(Flow* new_flow, const TECHNIQUE tech) {
  // Wrapper around the generic shortest path callback.
  function<double(Edge*)> cost_func = [&](Edge* edge) {
    return getEdgeCost(edge, tech);
  };
  Path* new_path = new Path(ComputeShortestPathGeneric(topo_, new_flow, cost_func));
  new_flow->AddPath(new_path);
  paths_map_[new_path] = new_flow;
  for(Edge* const edge : new_path->GetEdges()) {
    edges_map_[edge].push_back(new_path);
  }
}

double ShortestPathRouter::getEdgeCost(Edge* const edge, const TECHNIQUE tech) {
  switch(tech_) {
    case TECHNIQUE::BY_HOPS: return 1.0;
    case TECHNIQUE::BY_INVERSE_CAPACITY: return (1.0 / edge->GetCap());
    case TECHNIQUE::VIRTUAL_FUNCTION_CALL: return getEdgeCost(edge);
    default:
      assert(false);
  }
}

// This function should not be called unless a child class reimplements it.
double ShortestPathRouter::getEdgeCost(Edge* const edge) {
  assert(false);
  return 0;
}

void ShortestPathRouter::PostFlow(Flow flow) {
  // cout << "PostFlow()" << endl;

  assert(flow.GetSrc() != flow.GetDst());
  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  ComputeShortestPath(new_flow, tech_);

  VerifyConsistency();
}

} // namespace Network