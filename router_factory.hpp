// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef ROUTER_FACTORY_HPP
#define ROUTER_FACTORY_HPP

#include <cassert>
#include <vector>

#include "flow_router.hpp"
#include "bwr_router.hpp"
#include "shortest_path_router.hpp"
#include "utilization_router.hpp"

using namespace std;

namespace Network {

class RouterFactory {
public:
  RouterFactory() = delete;
  ~RouterFactory() = delete;

  enum class RouterType {
    BWR_ROUTER_BWROPT,
    BWR_ROUTER_BWRHF,
    SHORTEST_PATH_ROUTER_BY_HOPS,
    SHORTEST_PATH_ROUTER_BY_INVERSE_CAPACITY,
    UTILIZATION_ROUTER,
  };

  static FlowRouter* BuildRouter(RouterType router_type, Topology* topo) {
    switch(router_type) {
      case RouterType::BWR_ROUTER_BWROPT:
        return new BWRRouter(topo, BWRRouter::TECHNIQUE::BWROPT);
        break;
      case RouterType::BWR_ROUTER_BWRHF:
        return new BWRRouter(topo, BWRRouter::TECHNIQUE::BWRHF);
        break;
      case RouterType::SHORTEST_PATH_ROUTER_BY_HOPS:
        return new ShortestPathRouter(topo, ShortestPathRouter::TECHNIQUE::BY_HOPS);
        break;
      case RouterType::SHORTEST_PATH_ROUTER_BY_INVERSE_CAPACITY:
        return new ShortestPathRouter(topo, ShortestPathRouter::TECHNIQUE::BY_INVERSE_CAPACITY);
        break;
      case RouterType::UTILIZATION_ROUTER:
        return new UtilizationRouter(topo);
        break;
      default:
        assert(false);
    }
  }
};

} // namespace Network

#endif // ROUTER_FACTORY_HPP