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
    BWR_ROUTER_CAPTURE_K,
    BWR_ROUTER_CAPTURE_AND_PRUNE,
    BWR_ROUTER_INSTALL_AS_YOU_GO,
    SHORTEST_PATH_ROUTER_BY_HOPS,
    SHORTEST_PATH_ROUTER_BY_INVERSE_CAPACITY,
    UTILIZATION_ROUTER,
  };

  static FlowRouter* BuildRouter(RouterType router_type, int K, Topology* topo) {
    switch(router_type) {
      case RouterType::BWR_ROUTER_CAPTURE_K:
        return new BWRRouter(K, topo, BWRRouter::TECHNIQUE::CAPTURE_K);
        break;
      case RouterType::BWR_ROUTER_CAPTURE_AND_PRUNE:
        return new BWRRouter(K, topo, BWRRouter::TECHNIQUE::CAPTURE_AND_PRUNE);
        break;
      case RouterType::BWR_ROUTER_INSTALL_AS_YOU_GO:
        return new BWRRouter(K, topo, BWRRouter::TECHNIQUE::INSTALL_AS_YOU_GO);
        break;
      case RouterType::SHORTEST_PATH_ROUTER_BY_HOPS:
        return new ShortestPathRouter(K, topo, ShortestPathRouter::TECHNIQUE::BY_HOPS);
        break;
      case RouterType::SHORTEST_PATH_ROUTER_BY_INVERSE_CAPACITY:
        return new ShortestPathRouter(K, topo, ShortestPathRouter::TECHNIQUE::BY_INVERSE_CAPACITY);
        break;
      case RouterType::UTILIZATION_ROUTER:
        return new UtilizationRouter(K, topo);
        break;
      default:
        assert(false);
    }
  }
};

} // namespace Network

#endif // ROUTER_FACTORY_HPP