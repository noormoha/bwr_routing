// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef UTILIZATION_ROUTER_HPP
#define UTILIZATION_ROUTER_HPP

#include <vector>

#include "flow_router.hpp"
#include "shortest_path_router.hpp"
#include "tools.hpp"
#include "topology.hpp"

using namespace std;

namespace Network {

constexpr int MAX_PATH_LEN = 1000;

class UtilizationRouter : public ShortestPathRouter {
public:
	UtilizationRouter(Topology* topo);
protected:
	double power_base_;
	double getEdgeCost(Edge* const edge);
};

} // namespace Network

#endif // UTILIZATION_ROUTER_HPP