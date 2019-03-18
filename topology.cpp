// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "topology.hpp"

using namespace std;

namespace Network {
// Class "Node" methods.
Node::Node(int id) : id_(id) {}

int Node::GetID() {
  return id_;
}

// Class "Edge" methods.
Edge::Edge(Node* src, Node* dst, double capacity) : src_(src), dst_(dst), capacity_(capacity) {}

Node* Edge::GetSrc() {
  return src_;
}

Node* Edge::GetDst() {
  return dst_;
}

double Edge::GetCap() {
  return capacity_;
}

// Class "Topology" methods.
Topology::Topology(int nodes) : Topology(nodes, "") {}

Topology::Topology(int nodes, string name) : name_(name) {
  for(int id = 0; id < nodes; id++) {
    nodes_.push_back(new Node(id));
  }
}

void Topology::AddEdge(Node* src, Node* dst, double capacity) {
  Edge* edge = new Edge(src, dst, capacity);
  edges_.push_back(edge);
  adjlist_[src].push_back(make_pair(edge, dst));
  edgelookup_[make_pair(src, dst)] = edge;
}

void Topology::AddBidirectionalEdge(Node* src, Node* dst, double capacity) {
  AddEdge(src, dst, capacity);
  AddEdge(dst, src, capacity);
}

const vector<Node*>& Topology::GetNodes() {
  return nodes_;
}

const vector<Edge*>& Topology::GetEdges() {
  return edges_;
}

const vector< pair< Edge*, Node*> >& Topology::GetAdjList(Node* node) {
  return adjlist_[node];
}

Node* Topology::GetNode(int id) {
  return nodes_[id];
}

Edge* Topology::GetEdge(Node* src, Node* dst) {
  return edgelookup_[make_pair(src, dst)];
}

string Topology::GetName() {
  return name_;
}

Topology::~Topology() {
  for(Edge*& edge : edges_) {
    delete edge;
  }
  for(Node*& node : nodes_) {
    delete node;
  }
}

} // namespace Network