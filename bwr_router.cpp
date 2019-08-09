// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <queue>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <limits>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>

#include "bwr_router.hpp"

using namespace std;

namespace Network {
namespace {
  struct DijkPath {
    unordered_set<Edge*> path_edges;
    vector<Node*> path;
    unordered_set<Node*> visited;
    unordered_set<Path*> paths;
    double weight; // Weight of this path at the time of discovery. Needs to run set operation to update it if needed.
    DijkPath(unordered_set<Edge*> path_edges_, vector<Node*> path_, unordered_set<Path*> paths_, double weight_) : 
                        path_edges(move(path_edges_)), path(move(path_)), paths(move(paths_)), weight(weight_) {
      for(Node* const node : path) {
        visited.insert(node);
      }
    }
  };

  using DijkComp = function<bool(const DijkPath&, const DijkPath&)>;
}

// Get the weight for a path by using paths incident to it.
double BWRRouter::ComputePathWeight(const unordered_set<Path*>& incident_paths, 
                                    const unordered_set<Edge*>& path,
                                    const Flow* new_flow) {
  unordered_map<const Flow*, double> flow_to_bottleneck;
  for(Path* const incident_path : incident_paths) {
    const Flow* flow = paths_map_[incident_path];
    if(flow_to_bottleneck.find(flow) == flow_to_bottleneck.end()) {
      flow_to_bottleneck[flow] = numeric_limits<double>::max();
    }
    vector<Edge*> common_edges(path.size());
    set<Edge*> sorted_path(path.begin(), path.end()), 
               sorted_incident_path(incident_path->GetEdgesSet().begin(), incident_path->GetEdgesSet().end());
    auto it = set_intersection(sorted_path.begin(), sorted_path.end(),
      sorted_incident_path.begin(), sorted_incident_path.end(),
      common_edges.begin());
    common_edges.resize(it-common_edges.begin());
    // There has to be common edges.
    assert(common_edges.size() > 0);
    for(Edge* edge : common_edges) {
      flow_to_bottleneck[flow] = min(flow_to_bottleneck[flow], edge->GetCap());
    }
  }
  // Update next weight now.
  double next_weight = 0.0;
  for(pair<const Flow* const, double> flow_to_bottleneck_pair : flow_to_bottleneck) {
    // cout << flow_to_bottleneck_pair.first->GetRemainingSize() << "-" << flow_to_bottleneck_pair.second << endl;
    next_weight += flow_to_bottleneck_pair.first->GetRemainingSize() 
                      / flow_to_bottleneck_pair.second;
  }
  // Add the cost for current path
  double bottleneck = numeric_limits<double>::max();
  for(Edge* edge : path) {
    bottleneck = min(bottleneck, edge->GetCap());
  }
  next_weight += new_flow->GetRemainingSize() / bottleneck;
  return next_weight;
}

// This function find a path using the BWR optimal method (exhastive search but using a heap which makes it run faster).
// Please see the research paper for more info.
// The incoming flow object has only its size_ and id_ fields set, the rest is empty.
void BWRRouter::FindPathBWROpt(Flow* new_flow) {
  Node* const src = new_flow->GetSrc();
  Node* const dst = new_flow->GetDst();

  priority_queue<DijkPath, vector<DijkPath>, DijkComp> pq(
    [&](const DijkPath& path1, const DijkPath& path2) {
      return path1.weight > path2.weight;
  });
  pq.push(DijkPath({}, {src}, {}, 0.0));

  // solution of the routing algorithm
  DijkPath pq_sol({}, {src}, {}, 0.0);
  
  unordered_map<int, vector<DijkPath>> sol_by_hops;

  while( !pq.empty() ) {
    DijkPath current = pq.top();
    pq.pop();
    // If the path ends at the destination it is complete.
    if(current.path.back() == dst) {
      pq_sol = current;
      break;
    }
    // Update the heap.
    for(const pair<Edge* const, Node*>& next : topo_->GetAdjList(current.path.back())) {
      if(current.visited.find(next.second) == current.visited.end()) {
        // Calculate the next path's weight
        unordered_set<Path*> incident_paths(current.paths);
        for(Path* path : edges_map_[next.first]) {
          incident_paths.insert(path);
        }
        double next_weight = ComputePathWeight(incident_paths, current.path_edges, new_flow);
        unordered_set<Edge*> next_path_edges(current.path_edges);
        next_path_edges.insert(next.first);
        vector<Node*> next_path(current.path);
        next_path.push_back(next.second);
        DijkPath next_dijk(next_path_edges, next_path, incident_paths, next_weight);
        pq.push(next_dijk);
      }
    }
  }
  Path output(new_flow->GetID());
  for(int i = 1; i < pq_sol.path.size(); i++) {
    output.AddEdge(topo_->GetEdge(pq_sol.path[i-1], pq_sol.path[i]));
  }
  InstallPath(new_flow, output);
}

// This implements the BWRHF heuristic that is basically Dijkstra with weights assigned according to flow sizes.
void BWRRouter::FindPathBWRHF(Flow* new_flow) {
  // Wrapper around the generic shortest path callback.
  function<double(Edge*)> cost_func = [&](Edge* edge) {
    // Compute edge cost here.
    double edge_cost = (new_flow->GetRemainingSize() / edge->GetCap());
    for(Path* const path : edges_map_[edge]) {
      Flow* const flow = paths_map_.find(path)->second;
      edge_cost += (flow->GetRemainingSize() / edge->GetCap());
    }
    return edge_cost;
  };
  Path* new_path = new Path(ComputeShortestPathGeneric(topo_, new_flow, cost_func));
  new_flow->AddPath(new_path);
  paths_map_[new_path] = new_flow;
  for(Edge* const edge : new_path->GetEdges()) {
    edges_map_[edge].push_back(new_path);
  }
}

void BWRRouter::InstallPath(Flow* new_flow, const Path& path) {
  Path* new_path = new Path(path);
  new_flow->AddPath(new_path);
  paths_map_[new_path] = new_flow;
  for(Edge* const edge : new_path->GetEdges()) {
    edges_map_[edge].push_back(new_path);
  }
}

void BWRRouter::PostFlow(Flow flow) {
  assert(flow.GetSrc() != flow.GetDst());

  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  switch(tech_) {
    case TECHNIQUE::BWROPT: {
        FindPathBWROpt(new_flow);
        break;
      }
    case TECHNIQUE::BWRHF: {
        FindPathBWRHF(new_flow);
        break;
      }
    default:
      assert(false);
  }

  VerifyConsistency();
}

} // namespace Network