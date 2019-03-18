// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

namespace Network {

class Node {
public:
  explicit Node(int id);
  int GetID();
private:
  int id_;
};

class Edge {
public:
  Edge(Node* src, Node* dst, double capacity);
  Node* GetSrc();
  Node* GetDst();
  double GetCap();
private:
  Node *src_, *dst_;
  double capacity_;
};

// Represents a directed graph.
class Topology {
public:
  explicit Topology(int nodes);
  Topology(int nodes, string name);
  ~Topology();
  void AddEdge(Node* src, Node* dst, double capacity);
  void AddBidirectionalEdge(Node* src, Node* dst, double capacity);
  const vector<Node*>& GetNodes();
  const vector<Edge*>& GetEdges();
  const vector<pair<Edge*, Node*> >& GetAdjList(Node* node);
  Edge* GetEdge(Node* src, Node* dst);
  Node* GetNode(int id);
  string GetName();
private:
  const string name_;
  unordered_map< Node*, vector< pair<Edge*, Node*> > > adjlist_; // Node to next <Edge, Node>
  map<pair<Node*, Node*>, Edge*> edgelookup_; // node pair to edge in between
  vector<Edge*> edges_;
  vector<Node*> nodes_;
};

} // namespace Network

#endif // TOPOLOGY_HPP