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

#include "utilization_router.hpp"

using namespace std;

namespace Network {

UtilizationRouter::UtilizationRouter(int K, Topology* topo) : 
          power_base_(log(numeric_limits<double>::max() / MAX_PATH_LEN)),
          ShortestPathRouter(K, topo, ShortestPathRouter::TECHNIQUE::VIRTUAL_FUNCTION_CALL) {}

double UtilizationRouter::getEdgeCost(Edge* const edge) {
  return exp(GetEdgeUtilization(edge) * power_base_);
}

} // namespace Network