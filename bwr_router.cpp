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

#include "bwr_router.hpp"

using namespace std;

namespace Network {
namespace {
  struct DijkPath {
    vector<Node*> path;
    unordered_set<Node*> visited;
    unordered_set<Path*> paths;
    double weight; // Weight of this path at the time of discovery. Needs to run set operation to update it if needed.
    DijkPath(vector<Node*> path_, unordered_set<Path*> paths_, double weight_) : 
                        path(move(path_)), paths(move(paths_)), weight(weight_) {
      for(Node* const node : path) {
        visited.insert(node);
      }
    }
  };

  using DijkComp = function<bool(const DijkPath&, const DijkPath&)>;
}

// Get the weight for a given path by first computing all incident paths.
double BWRRouter::ComputePathWeight(const Path* path) {
  unordered_set<Path*> incident_paths;
  for(Edge* const edge : path->GetEdges()) {
    for(Path* incident_path : edges_map_[edge]) {
      incident_paths.insert(incident_path);
    }
  }
  return ComputePathWeight(incident_paths);
}

// Get the weight for a path by using paths incident to it.
double BWRRouter::ComputePathWeight(const unordered_set<Path*>& incident_paths) {
  unordered_map<const Flow*, double> flow_to_bottleneck;
  for(Path* const path : incident_paths) {
    const Flow* flow = paths_map_[path];
    if(flow_to_bottleneck.find(flow) == flow_to_bottleneck.end()) {
      flow_to_bottleneck[flow] = numeric_limits<double>::max();
    }
    flow_to_bottleneck[flow] = min(flow_to_bottleneck[flow], path->GetBottleneckCap());
  }
  // Update next weight now.
  double next_weight = 0.0;
  for(pair<const Flow* const, double> flow_to_bottleneck_pair : flow_to_bottleneck) {
    // cout << flow_to_bottleneck_pair.first->GetRemainingSize() << "-" << flow_to_bottleneck_pair.second << endl;
    next_weight += flow_to_bottleneck_pair.first->GetRemainingSize() 
                      / flow_to_bottleneck_pair.second;
  }
  return next_weight;
}

// This function find a path using the BWRH method.
// Please see the research paper for more info.
// The incoming flow object has only its size_ and id_ fields set, the rest is empty.

void BWRRouter::FindPathBWRH(Flow* new_flow) {
  Node* const src = new_flow->GetSrc();
  Node* const dst = new_flow->GetDst();

  priority_queue<DijkPath, vector<DijkPath>, DijkComp> pq(
    [&](const DijkPath& path1, const DijkPath& path2) {
      return path1.path.size() > path2.path.size();
  });
  pq.push(DijkPath({src}, {}, 0.0));

  // solution of the routing algorithm
  DijkPath pq_sol({src}, {}, 0.0);
  
  unordered_map<int, vector<DijkPath>> sol_by_hops;

  while( !pq.empty() && pq_sol.path.size() < 2 ) {
    DijkPath current = pq.top();
    pq.pop();
    // End condition to be checked here ------------------
    // That is, we do not see any improvement in the optimal solution for additional hops.
    if(pq_sol.path.size() > 1 && current.path.size() > 1) {
      // If no new solution for these number of hops kill the loop.
      if(sol_by_hops[current.path.size()-1].empty() && 
         sol_by_hops[current.path.size()].empty()) {
        // {
        //   cout << "Terminated at: ";
        //   for(Node* node : current.path) {
        //     cout << node->GetID() << "-";
        //   }
        //   cout << "?" << endl;
        // }
        break;
      }
    }
    // If the path ends at the destination it is complete.
    if(current.path.back() == dst) {
      // This path is complete.
      if(pq_sol.path.size() < 2 || current.weight < pq_sol.weight) {
        pq_sol = current;
        sol_by_hops[current.path.size()].push_back(current);
      }
      continue;
    }
    // Update the heap.
    for(const pair<Edge* const, Node*>& next : topo_->GetAdjList(current.path.back())) {
      if(current.visited.find(next.second) == current.visited.end()) {
        // Calculate the next path's weight
        unordered_set<Path*> incident_paths(current.paths);
        for(Path* path : edges_map_[next.first]) {
          incident_paths.insert(path);
        }
        double next_weight = ComputePathWeight(incident_paths);
        // {
        //   cout << next_weight << " for path: ";
        //   for(Node* node : current.path) {
        //     cout << node->GetID() << "-";
        //   }
        //   cout << next.second->GetID() << endl;
        // }
        // This acts like branch-and-cut: if the solution is already bad, cut it off.
        if(pq_sol.path.size() < 2 || next_weight < pq_sol.weight) {
          vector<Node*> next_path(current.path);
          next_path.push_back(next.second);
          DijkPath next_dijk(next_path, incident_paths, next_weight);
          pq.push(next_dijk);
        }
      }
    }
  }
  Path output(new_flow->GetID());
  for(int i = 1; i < pq_sol.path.size(); i++) {
    output.AddEdge(topo_->GetEdge(pq_sol.path[i-1], pq_sol.path[i]));
  }
  InstallPath(new_flow, output);
}

void BWRRouter::FindPathBWRHF(Flow* new_flow) {
  Node* const src = new_flow->GetSrc();
  Node* const dst = new_flow->GetDst();


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
  // cout << "PostFlow()" << endl;

  assert(flow.GetSrc() != flow.GetDst());
  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  switch(tech_) {
    case TECHNIQUE::BWRH: {
        FindPathBWRH(new_flow);
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