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
    DijkPath(vector<Node*> path_, unordered_set<Path*> paths_, double weight_) : path(move(path_)), paths(move(paths_)), weight(weight_) {
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

// This function routes the new flow according to the routing policy.
// The incoming flow object has only its size_ and id_ fields set, the rest is empty.
// There are two approaches that we can use:
// Approach 1: select minimum weight paths not considering the effect of this (self) flow.
// Approach 2: select as many paths as there are given the termination condition (no better 
//  path than the best yet found for a given number of hops), then start from the best path 
//  in the heap and keep adding the paths until we get exactly K_ paths.
// Approach 3: similar to Approach 2, but consider the effect of paths already selected. This
//  means the paths are selected as they are found, so if a better path exists we my not see them.

vector<Path> BWRRouter::ComputeBasePaths(Flow* new_flow, bool install_as_you_go) {
  Node* const src = new_flow->GetSrc();
  Node* const dst = new_flow->GetDst();

  priority_queue<DijkPath, vector<DijkPath>, DijkComp> pq(
    [&](const DijkPath& path1, const DijkPath& path2) {
      return path1.path.size() > path2.path.size();
  });
  pq.push(DijkPath({src}, {}, 0.0));

  // min heap (lowest weight up)
  priority_queue<DijkPath, vector<DijkPath>, DijkComp> pq_sols(
    [&](const DijkPath& path1, const DijkPath& path2) {
      return (path1.weight > path2.weight) ||
             (abs(path1.weight - path2.weight) < 1 && (path1.path.size() > path2.path.size()));
  });
  
  unordered_map<int, vector<DijkPath>> sol_by_hops;

  // How many extra paths to search for? "pq_sols.size() < K_" determines how far we're willing to go!
  // This comes at the cost of computation: if we kill as soon as we have K_ paths we get shorter paths.
  // However, we might miss some better longer paths. Let's search for 10*K_ for now to limit the running time.
  while( !pq.empty() && 
        (pq_sols.size() < (install_as_you_go ? K_ : 10*K_)) ) {
    DijkPath current = pq.top();
    pq.pop();
    if(install_as_you_go) {
      // Compute incident paths and new path weight for the new path.
      Path current_path(new_flow->GetID());
      for(int i = 1; i < current.path.size(); i++) {
        current_path.AddEdge(topo_->GetEdge(current.path[i-1], current.path[i]));
      }
      current.weight = ComputePathWeight(&current_path);
      // {
      //   cout << current.weight << " for path: ";
      //   for(Node* node : current.path) {
      //     cout << node->GetID() << "-";
      //   }
      //   cout << "?" << endl;
      // }
      if(!pq_sols.empty() && current.weight > pq_sols.top().weight) {
        continue;
      }
    }
    // End condition to be checked here ------------------
    // That is, we do not see any improvement in the optimal solution for additional hops.
    if(pq_sols.size() > 0 && current.path.size() > 1) {
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
      if(pq_sols.empty() || (!pq_sols.empty() && current.weight <= pq_sols.top().weight)) {
        pq_sols.push(current);
        sol_by_hops[current.path.size()].push_back(current);
        if(install_as_you_go) {
          // Install this path and find the next path given the effect of this one
          Path new_path(new_flow->GetID());
          for(int i = 1; i < current.path.size(); i++) {
            new_path.AddEdge(topo_->GetEdge(current.path[i-1], current.path[i]));
          }
          InstallPath(new_flow, new_path);
        }
      }
      continue;
    }
    // Update the heap.
    for(const pair<Edge* const, Node*>& next : topo_->GetAdjList(current.path.back())) {
      if(current.visited.find(next.second) == current.visited.end()) {
        if(install_as_you_go) {
          vector<Node*> next_path(current.path);
          next_path.push_back(next.second);
          DijkPath next_dijk(next_path, {}, numeric_limits<double>::max());
          pq.push(next_dijk);
        } else {
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
          if(pq_sols.empty() || (!pq_sols.empty() && next_weight <= pq_sols.top().weight)) {
            vector<Node*> next_path(current.path);
            next_path.push_back(next.second);
            DijkPath next_dijk(next_path, incident_paths, next_weight);
            pq.push(next_dijk);
          }
        }
      }
    }
  }
  if(install_as_you_go) {
    return vector<Path>{}; // We have already installed the paths!
  }
  vector<Path> output;
  while(!pq_sols.empty()) {
    const DijkPath& sol = pq_sols.top();
    Path new_path(new_flow->GetID());
    for(int i = 1; i < sol.path.size(); i++) {
      new_path.AddEdge(topo_->GetEdge(sol.path[i-1], sol.path[i]));
    }
    output.push_back(new_path);
    pq_sols.pop();
  }
  return output;
}

void BWRRouter::InstallPath(Flow* new_flow, const Path& path) {
  Path* new_path = new Path(path);
  new_flow->AddPath(new_path);
  paths_map_[new_path] = new_flow;
  for(Edge* const edge : new_path->GetEdges()) {
    edges_map_[edge].push_back(new_path);
  }
}

// Approach 1: ................................................
void BWRRouter::CaptureK(Flow* new_flow, vector<Path>& paths) {
  for(int i = 0; i < min((int) paths.size(), K_); i++) {
    InstallPath(new_flow, paths[i]);
  }
}

// Approach 2: ................................................
void BWRRouter::CaptureAndPrune(Flow* new_flow, vector<Path>& paths) {
  if(paths.size() <= K_) {
    CaptureK(new_flow, paths);
  } else {
    // we need to drop some paths to get exactly K_
    // i == 0 is the best path originally which will be added first
    pair<int, double> best_sol = {0, -1};
    for(int i = 0; i < K_ - 1; i++) {
      InstallPath(new_flow, paths[best_sol.first]);
      paths.erase(paths.begin() + best_sol.first);
      best_sol.second = numeric_limits<double>::max();
      for(int j = 0; j < paths.size(); j++) {
        double next_weight = ComputePathWeight(&paths[j]);
        if(next_weight < best_sol.second) {
          best_sol = {j, next_weight};
        }
      }
    }
    InstallPath(new_flow, paths[best_sol.first]);
  }
}

void BWRRouter::PostFlow(Flow flow) {
  // cout << "PostFlow()" << endl;

  assert(flow.GetSrc() != flow.GetDst());
  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  switch(tech_) {
    case TECHNIQUE::CAPTURE_K: {
        vector<Path> paths = ComputeBasePaths(new_flow, false);
        CaptureK(new_flow, paths);
        break;
      }
    case TECHNIQUE::CAPTURE_AND_PRUNE: {
        vector<Path> paths = ComputeBasePaths(new_flow, false);
        CaptureAndPrune(new_flow, paths);
        break;
      }
    case TECHNIQUE::INSTALL_AS_YOU_GO: {
        ComputeBasePaths(new_flow, true);
        break;
      }
    default:
      assert(false);
  }

  VerifyConsistency();
}

} // namespace Network