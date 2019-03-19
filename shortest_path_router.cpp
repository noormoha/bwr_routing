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

#include "shortest_path_router.hpp"

using namespace std;

namespace Network {
namespace {
  struct DijkPath {
    vector<Node*> path;
    unordered_set<Node*> visited;
    double weight;
    // Constructor.
    DijkPath(vector<Node*> path_, double weight_) : 
             path(move(path_)), weight(weight_) {
      for(Node* const node : path) {
        visited.insert(node);
      }
    }
  };

  using DijkComp = function<bool(const DijkPath&, const DijkPath&)>;
}

void ShortestPathRouter::ComputeKShortestPaths(Flow* new_flow, const int K, const TECHNIQUE tech) {
  Node* const src = new_flow->GetSrc();
  Node* const dst = new_flow->GetDst();

  priority_queue<DijkPath, vector<DijkPath>, DijkComp> pq(
    [&](const DijkPath& path1, const DijkPath& path2) {
      return path1.weight > path2.weight;
  });
  pq.push(DijkPath({src}, 0.0));

  vector<DijkPath> sols;
  unordered_map<Node*, double> num_paths_by_node;

  while(!pq.empty()) {
    DijkPath current = pq.top();
    pq.pop();
    num_paths_by_node[current.path.back()]++;
    // End condition to be checked here ------------------
    if(current.path.back() == dst) {
      sols.push_back(current);
      if(sols.size() == K) {
        break;
      }
      continue;
    }
    if(num_paths_by_node[current.path.back()] > K) {
      continue;
    }
    // Update the heap.
    for(const pair<Edge* const, Node*>& next : topo_->GetAdjList(current.path.back())) {
      if(current.visited.find(next.second) == current.visited.end()) {
        DijkPath new_path = current;
        new_path.path.push_back(next.second);
        new_path.visited.insert(next.second);
        new_path.weight += getEdgeCost(next.first, tech);
      }
    }
  }

  while(!sols.empty()) {
    const DijkPath& sol = sols.back();
    Path* new_path = new Path(new_flow->GetID());
    for(int i = 1; i < sol.path.size(); i++) {
      new_path->AddEdge(topo_->GetEdge(sol.path[i-1], sol.path[i]));
    }
    new_flow->AddPath(new_path);
    paths_map_[new_path] = new_flow;
    for(Edge* const edge : new_path->GetEdges()) {
      edges_map_[edge].push_back(new_path);
    }
    sols.pop_back();
  }
}

double ShortestPathRouter::getEdgeCost(Edge* const edge, const TECHNIQUE tech) {
  switch(tech_) {
    case TECHNIQUE::BY_HOPS: return 1.0;
    case TECHNIQUE::BY_INVERSE_CAPACITY: return (1.0 / edge->GetCap());
    default:
      assert(false);
  }
}

void ShortestPathRouter::PostFlow(Flow flow) {
  // cout << "PostFlow()" << endl;

  assert(flow.GetSrc() != flow.GetDst());
  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  ComputeKShortestPaths(new_flow, K_, tech_);

  VerifyConsistency();
}

} // namespace Network