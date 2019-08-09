// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef BWR_ROUTER_HPP
#define BWR_ROUTER_HPP

#include <vector>

#include "flow_router.hpp"
#include "tools.hpp"
#include "topology.hpp"

using namespace std;

namespace Network {

class BWRRouter : public FlowRouter {
public:
  enum class TECHNIQUE {
    BWROPT, BWRHF
  };
  BWRRouter(Topology* topo, TECHNIQUE tech) : 
            FlowRouter(topo), tech_(tech) {}
  void PostFlow(Flow flow);
protected:
  TECHNIQUE tech_;
  void FindPathBWROpt(Flow* new_flow);
  void FindPathBWRHF(Flow* new_flow);
  void CaptureK(Flow* new_flow, vector<Path>& paths);
  void CaptureAndPrune(Flow* new_flow, vector<Path>& paths);
  double ComputePathWeight(const unordered_set<Path*>& incident_paths, 
    const unordered_set<Edge*>& path, const Flow* new_flow);
  // double ComputePathWeight(const Path* path);
  void InstallPath(Flow* new_flow, const Path& path);
};

} // namespace Network

#endif // BWR_ROUTER_HPP