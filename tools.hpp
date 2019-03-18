// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <unordered_set>
#include <vector>

#include "topology.hpp"

using namespace std;

namespace Network {

class Path {
public:
  explicit Path(int flow_id);
  void AddEdge(Edge* edge);
  const vector<Edge*>& GetEdges() const;
  const unordered_set<Edge*>& GetEdgesSet() const;
  double GetBottleneckCap();
  bool operator==(const Path& path);
  const int GetFlow() const;
private:
  int flow_id_;
  vector<Edge*> edges_;
  unordered_set<Edge*> edges_hashset_;
  double bottleneck_cap_;
};

bool operator==(const Path& path1, const Path& path2);

class Flow {
public:
  Flow(int id, Node* const src, Node* const dst, double size);
  void AddPath(Path* path);
  const vector<Path*>& GetPaths();
  double GetRemainingSize() const;
  void AddCompleted(double completed);
  Node* GetSrc();
  Node* GetDst();
  int GetID();
private:
  int id_;
  double size_;
  Node* const src_;
  Node* const dst_;
  double completed_;
  vector<Path*> paths_;
};

} // namespace Network

#endif // TOOLS_HPP