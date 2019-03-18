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


void UtilizationRouter::PostFlow(Flow flow) {
  // cout << "PostFlow()" << endl;

  assert(flow.GetSrc() != flow.GetDst());
  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  // The actual routing algorithm.
  

  VerifyConsistency();
}

} // namespace Network