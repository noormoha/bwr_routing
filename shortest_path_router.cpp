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

void ShortestPathRouter::ComputeShortestPath(Flow* new_flow, const TECHNIQUE tech) {
  Node* const src = new_flow->GetSrc();
  Node* const dst = new_flow->GetDst();

  priority_queue<DijkPath, vector<DijkPath>, DijkComp> pq(
    [&](const DijkPath& path1, const DijkPath& path2) {
      return path1.weight > path2.weight;
  });
  pq.push(DijkPath({src}, 0.0));

  DijkPath sol({src}, 0.0);
  unordered_map<Node*, double> num_paths_by_node;

  while(!pq.empty()) {
    DijkPath current = pq.top();
    pq.pop();
    num_paths_by_node[current.path.back()]++;
    // End condition to be checked here ------------------
    if(current.path.back() == dst) {
      sol = current;
      break;
    }
    if(num_paths_by_node[current.path.back()] > 1) {
      continue;
    }
    // Update the heap.
    for(const pair<Edge* const, Node*>& next : topo_->GetAdjList(current.path.back())) {
      if(current.visited.find(next.second) == current.visited.end()) {
        DijkPath new_path = current;
        new_path.path.push_back(next.second);
        new_path.visited.insert(next.second);
        const double edge_cost = getEdgeCost(next.first, tech);
        assert(numeric_limits<double>::max() - edge_cost > new_path.weight);
        assert(numeric_limits<double>::max() - new_path.weight > edge_cost);
        new_path.weight += edge_cost;
        pq.push(new_path);
      }
    }
  }
  Path* new_path = new Path(new_flow->GetID());
  for(int i = 1; i < sol.path.size(); i++) {
    new_path->AddEdge(topo_->GetEdge(sol.path[i-1], sol.path[i]));
  }
  new_flow->AddPath(new_path);
  paths_map_[new_path] = new_flow;
  for(Edge* const edge : new_path->GetEdges()) {
    edges_map_[edge].push_back(new_path);
  }
}

double ShortestPathRouter::getEdgeCost(Edge* const edge, const TECHNIQUE tech) {
  switch(tech_) {
    case TECHNIQUE::BY_HOPS: return 1.0;
    case TECHNIQUE::BY_INVERSE_CAPACITY: return (1.0 / edge->GetCap());
    case TECHNIQUE::VIRTUAL_FUNCTION_CALL: return getEdgeCost(edge);
    default:
      assert(false);
  }
}

// This function should not be called unless a child class reimplements it.
double ShortestPathRouter::getEdgeCost(Edge* const edge) {
  assert(false);
  return 0;
}

void ShortestPathRouter::PostFlow(Flow flow) {
  // cout << "PostFlow()" << endl;

  assert(flow.GetSrc() != flow.GetDst());
  Flow* new_flow = new Flow(flow);
  flows_map_[new_flow->GetID()] = new_flow;

  ComputeShortestPath(new_flow, tech_);

  VerifyConsistency();
}

} // namespace Network