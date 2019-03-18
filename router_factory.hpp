// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef ROUTER_FACTORY_HPP
#define ROUTER_FACTORY_HPP

#include <cassert>
#include <vector>

#include "flow_router.hpp"
#include "bwr_router.hpp"

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
      default:
        assert(false);
    }
  }
};

} // namespace Network

#endif // ROUTER_FACTORY_HPP