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

#include "tools.hpp"

using namespace std;

namespace Network {

Path::Path(int flow_id) : flow_id_(flow_id), bottleneck_cap_(numeric_limits<double>::max()) {}

void Path::AddEdge(Edge* edge) {
  edges_.push_back(edge);
  edges_hashset_.insert(edge);
  bottleneck_cap_ = min(bottleneck_cap_, edge->GetCap());
}

const vector<Edge*>& Path::GetEdges() const {
  return edges_;
}

const unordered_set<Edge*>& Path::GetEdgesSet() const {
  return edges_hashset_;
}

double Path::GetBottleneckCap() {
  return bottleneck_cap_;
}

const int Path::GetFlow() const {
  return flow_id_;
}

bool Path::operator==(const Path& path) {
  return (edges_hashset_ == path.GetEdgesSet()) && (flow_id_ == path.GetFlow());
}

bool operator==(const Path& path1, const Path& path2) {
  return (path1.GetEdgesSet() == path2.GetEdgesSet()) && (path1.GetFlow() == path2.GetFlow());
}

Flow::Flow(int id, Node* const src, Node* const dst, double size) : 
           id_(id), src_(src), dst_(dst), size_(size), completed_(0.0) {}

void Flow::AddPath(Path* path) {
  paths_.push_back(path);
}

const vector<Path*>& Flow::GetPaths() {
  return paths_;
}

double Flow::GetRemainingSize() const {
  return max(size_ - completed_, 0.0);
}

void Flow::AddCompleted(double completed) {
  completed_ += completed;
}

Node* Flow::GetSrc() {
  return src_;
}

Node* Flow::GetDst() {
  return dst_;
}

int Flow::GetID() {
  return id_;
}

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

Path ComputeShortestPathGeneric(Topology* topo, Flow* new_flow, function<double(Edge*)> cost_func) {
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
    for(const pair<Edge* const, Node*>& next : topo->GetAdjList(current.path.back())) {
      if(current.visited.find(next.second) == current.visited.end()) {
        DijkPath new_path = current;
        new_path.path.push_back(next.second);
        new_path.visited.insert(next.second);
        const double edge_cost = cost_func(next.first);
        assert(numeric_limits<double>::max() - edge_cost > new_path.weight);
        assert(numeric_limits<double>::max() - new_path.weight > edge_cost);
        new_path.weight += edge_cost;
        pq.push(new_path);
      }
    }
  }
  Path new_path(new_flow->GetID());
  for(int i = 1; i < sol.path.size(); i++) {
    new_path.AddEdge(topo->GetEdge(sol.path[i-1], sol.path[i]));
  }
  return new_path;
}

} // namespace Network