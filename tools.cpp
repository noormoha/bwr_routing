// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <unordered_set>
#include <vector>
#include <limits>

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

} // namespace Network