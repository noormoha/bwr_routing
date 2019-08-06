// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef FLOW_ROUTER_HPP
#define FLOW_ROUTER_HPP

#include <unordered_map>

#include "tools.hpp"

namespace Network {

// Timeslot duration in seconds.
constexpr double TIMESLOT_DURATION = 1;

// FlowRouter is an interface that all routers need to implement. It offers 
// a unique set of access points to the flow routing functions shared by
// all flow routing techniques.
class FlowRouter {
public:
  FlowRouter(Topology* topo) : topo_(topo), time_(0.0) {}
  ~FlowRouter();
  // Implementted by the underlying routing policy.
  virtual void PostFlow(Flow flow) = 0;
  // Compute the transmission rates according to the rate-allocation policy.
  // Assume data transmission with the computed rates for duration of one time unit.
  // Updated flow demands according to what was transmitted.
  unordered_map<Path*, double> NextSlot();
  // Extract flow completion times from this flow router object. Only flows completed to this
  // point will be reported.
  vector<double> GetCompletionTimes();
  // Check whether there is any incomplete flows.
  int getRemainingFlows();
  // Obtain the simulation epoch.
  double getEpoch();
  // Verify consistency of stored data.
  void VerifyConsistency();
  // Get total remaining demand.
  double GetTotalRemainingDemand();
  // Get edge utilization.
  double GetEdgeUtilization(Edge* const edge) const;
protected:
  double time_; // The current timeslot.
  unordered_map<int, Flow*> flows_map_; // Flow id to flow pointer.
  unordered_map<Path*, Flow*> paths_map_; // Get the flow pointer associated with a path.
  unordered_map<Edge*, vector<Path*> > edges_map_; // Edge pointer to paths on that edge.
  // The time at which a flow was completed. When it happens, this flow is removed from all the lookup tables above.
  unordered_map<Flow*, double> flow_completion_times_;
  Topology* topo_; // The topology this router is associated with.
  // Utilization data for routing purposes.
  // Max: 1.0, Min: 0.0
  unordered_map<Edge*, double> edge_utilization_;
};

} // namespace Network

#endif // FLOW_ROUTER_HPP