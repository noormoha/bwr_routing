// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef UTILIZATION_ROUTER_HPP
#define UTILIZATION_ROUTER_HPP

#include <vector>

#include "flow_router.hpp"
#include "tools.hpp"
#include "topology.hpp"

using namespace std;

namespace Network {

class UtilizationRouter : public FlowRouter {
public:
  UtilizationRouter(int K, Topology* topo) : 
            FlowRouter(K, topo) {}
  void PostFlow(Flow flow);
protected:
};

} // namespace Network

#endif // UTILIZATION_ROUTER_HPP