// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <iostream>

#include "tests.hpp"

using namespace std;

namespace Network {

void BWRRouterTest::RunTests() {
  cout << "Testing RouteFlow..." << endl;
  // Test 1: test the path equality functions.
  cout << "Test 1................................................" << endl;
  Path path3(0);
  path3.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(1)));
  path3.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(3)));
  Path path4(0);
  path4.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(3)));
  path4.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(1)));
  PathsPrint(&path3);
  PathsPrint(&path4);
  assert(PathsEqual(&path3, &path4));
  // Test 2: route some empty flows over the topology, should use the min-hop paths.
  cout << "Test 2................................................" << endl;
  Path path5(0);
  path5.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(2)));
  Path path6(0);
  path6.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(2)));
  path6.AddEdge(topo_->GetEdge(topo_->GetNode(4), topo_->GetNode(1)));
  PathsPrint(&path5);
  PathsPrint(&path6);
  assert(!PathsEqual(&path5, &path6));
  // Test 2.1: verify the bottleneck capacity of some paths.
  Path path7(0);
  path7.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(1)));
  path7.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(3)));
  assert(path7.GetBottleneckCap() == 0.2);
  path7.AddEdge(topo_->GetEdge(topo_->GetNode(3), topo_->GetNode(2)));
  assert(path7.GetBottleneckCap() == 0.2);
  // Test 2.2: verify the bottleneck capacity of some paths.
  Path path8(0);
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(4)));
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(4), topo_->GetNode(1)));
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(2)));
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(2), topo_->GetNode(3)));
  assert(path8.GetBottleneckCap() == 0.4);
  // Test 3: route an actual flow on the empty network, it should go with two min-hop paths.
  cout << "Test 3................................................" << endl;
  Flow flow_1(0, topo_->GetNode(0), topo_->GetNode(4), 100);
  PostFlow(flow_1);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  // Test 4: route another flow on the topology now given the other already existing flow.
  cout << "Test 4................................................" << endl;
  Flow flow_2(1, topo_->GetNode(0), topo_->GetNode(3), 5000);
  PostFlow(flow_2);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  // Test 5: compute fair rates given the topology and demands.
  unordered_map<Path*, double> allocations = NextSlot();
  for(auto& pair : allocations) {
    cout << "Rate: " << pair.second << " Path: ";
    PathsPrint(pair.first);
  }
  // Test 6: final flow to test rates.
  cout << "Test 6................................................" << endl;
  Flow flow_3(2, topo_->GetNode(2), topo_->GetNode(0), 300);
  PostFlow(flow_3);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  allocations = NextSlot();
  for(auto& pair : allocations) {
    cout << "Rate: " << pair.second << " Path: ";
    PathsPrint(pair.first);
  }
  // Test 7: final flow to test rates.
  cout << "Test 7................................................" << endl;
  Flow flow_4(3, topo_->GetNode(3), topo_->GetNode(2), 10);
  PostFlow(flow_4);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  allocations = NextSlot();
  for(auto& pair : allocations) {
    cout << "Rate: " << pair.second << " Path: ";
    PathsPrint(pair.first);
  }
}

void ShortestPathRouterTest::RunTests() {
  cout << "Testing RouteFlow..." << endl;
  // Test 1: test the path equality functions.
  cout << "Test 1................................................" << endl;
  Path path3(0);
  path3.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(1)));
  path3.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(3)));
  Path path4(0);
  path4.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(3)));
  path4.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(1)));
  PathsPrint(&path3);
  PathsPrint(&path4);
  assert(PathsEqual(&path3, &path4));
  // Test 2: route some empty flows over the topology, should use the min-hop paths.
  cout << "Test 2................................................" << endl;
  Path path5(0);
  path5.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(2)));
  Path path6(0);
  path6.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(2)));
  path6.AddEdge(topo_->GetEdge(topo_->GetNode(4), topo_->GetNode(1)));
  PathsPrint(&path5);
  PathsPrint(&path6);
  assert(!PathsEqual(&path5, &path6));
  // Test 2.1: verify the bottleneck capacity of some paths.
  Path path7(0);
  path7.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(1)));
  path7.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(3)));
  assert(path7.GetBottleneckCap() == 0.2);
  path7.AddEdge(topo_->GetEdge(topo_->GetNode(3), topo_->GetNode(2)));
  assert(path7.GetBottleneckCap() == 0.2);
  // Test 2.2: verify the bottleneck capacity of some paths.
  Path path8(0);
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(0), topo_->GetNode(4)));
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(4), topo_->GetNode(1)));
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(1), topo_->GetNode(2)));
  path8.AddEdge(topo_->GetEdge(topo_->GetNode(2), topo_->GetNode(3)));
  assert(path8.GetBottleneckCap() == 0.4);
  // Test 3: route an actual flow on the empty network, it should go with two min-hop paths.
  cout << "Test 3................................................" << endl;
  Flow flow_1(0, topo_->GetNode(0), topo_->GetNode(4), 100);
  PostFlow(flow_1);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  // Test 4: route another flow on the topology now given the other already existing flow.
  cout << "Test 4................................................" << endl;
  Flow flow_2(1, topo_->GetNode(0), topo_->GetNode(3), 5000);
  PostFlow(flow_2);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  // Test 5: compute fair rates given the topology and demands.
  unordered_map<Path*, double> allocations = NextSlot();
  for(auto& pair : allocations) {
    cout << "Rate: " << pair.second << " Path: ";
    PathsPrint(pair.first);
  }
  // Test 6: final flow to test rates.
  cout << "Test 6................................................" << endl;
  Flow flow_3(2, topo_->GetNode(2), topo_->GetNode(0), 300);
  PostFlow(flow_3);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  allocations = NextSlot();
  for(auto& pair : allocations) {
    cout << "Rate: " << pair.second << " Path: ";
    PathsPrint(pair.first);
  }
  // Test 7: final flow to test rates.
  cout << "Test 7................................................" << endl;
  Flow flow_4(3, topo_->GetNode(3), topo_->GetNode(2), 10);
  PostFlow(flow_4);
  for(auto& pair : paths_map_) {
    cout << "Flow ID (" << pair.second->GetID() << "): ";
    PathsPrint(pair.first);
  }
  for(auto& pair : edges_map_) {
    cout << "Edge = (" << pair.first->GetSrc()->GetID() << "-" << pair.first->GetDst()->GetID() << "): " << endl;
    for(auto& path : pair.second) {
      PathsPrint(path);
    }
  }
  allocations = NextSlot();
  for(auto& pair : allocations) {
    cout << "Rate: " << pair.second << " Path: ";
    PathsPrint(pair.first);
  }
}

bool PathsEqual(Path* p1, Path* p2) {
  return (*p1 == *p2);
}

void PathsPrint(Path* p) {
  for(Edge* const edge : p->GetEdges()) {
    cout << "[" << edge->GetSrc()->GetID() << "-" << edge->GetDst()->GetID() << "] ";
  }
  cout << endl;
}

Topology* BuildTopology() {
  const int Nodes = 5;
  Topology* topo = new Topology(Nodes, "Test_Topo_5_Nodes");
  vector<vector<pair<int, double>>> adj_list = {
    {{1, 0.2}, {4, 1.0}},
    {{0, 0.2}, {2, 1.8}, {3, 0.5}, {4, 1.5}},
    {{1, 1.8}, {3, 0.4}},
    {{1, 0.5}, {2, 0.4}},
    {{0, 1}, {1, 1.5}}
  };
  for(int i = 0; i < Nodes; i++) {
    for(pair<int, double>& next_weight_pair : adj_list[i]) {
      topo->AddEdge(topo->GetNode(i), topo->GetNode(next_weight_pair.first), next_weight_pair.second);
    }
  }
  return topo;
}

void TestDistribution(Stochastic::DistributionTypes dist_type) {
  Stochastic dist(1.0, 0.1, dist_type);
  pair<double, double> flow;
  int index = 0;
  double inter_arrival_avg = 0., size_avg = 0., last_arrival = 0.;
  while((flow = dist.nextSample()).first < 1E6) {
    // cout << "#" << (++index) << ", Arrival: " << flow.first << ", Size: " << flow.second << std::endl;
    index++;
    inter_arrival_avg += (flow.first - last_arrival);
    size_avg += flow.second;
    last_arrival = flow.first;
  }
  if(index > 0) {
    inter_arrival_avg /= index;
    size_avg /= index;
    cout << "inter_arrival_avg: " << inter_arrival_avg << ", size_avg: " << size_avg << endl;
    assert(abs(inter_arrival_avg - 1.0) < 1E-1); // Error less than 10%.
    assert(abs(size_avg - 10.0) < 1E-0); // Error less than 10%.
  }
}

void RunAllTests() {
  cout << "Running All Tests..." << endl;

  // Create a topology.
  Topology* test_topo = BuildTopology();
  int K = 2;

  // BWRRouter Test
  BWRRouterTest bwr_router_test(K, test_topo, BWRRouter::TECHNIQUE::CAPTURE_K);
  // Test the RouteFlow function from the parent class.
  bwr_router_test.RunTests();

  // BWRRouter Test
  BWRRouterTest bwr_router_test2(K, test_topo, BWRRouter::TECHNIQUE::INSTALL_AS_YOU_GO);
  // Test the RouteFlow function from the parent class.
  bwr_router_test2.RunTests();

  // ShortestPathRouter Test
  ShortestPathRouterTest shortest_path_router_test(K, test_topo, ShortestPathRouter::TECHNIQUE::BY_HOPS);
  // Test the RouteFlow function from the parent class.
  shortest_path_router_test.RunTests();

  // ShortestPathRouter Test
  ShortestPathRouterTest shortest_path_router_test2(K, test_topo, ShortestPathRouter::TECHNIQUE::BY_INVERSE_CAPACITY);
  // Test the RouteFlow function from the parent class.
  shortest_path_router_test2.RunTests();

  // Delete topology.
  delete test_topo;

  // Generate some random flows.
  TestDistribution(Stochastic::DistributionTypes::DIST_EXPONENTIAL);
  TestDistribution(Stochastic::DistributionTypes::DIST_PARETO);
  TestDistribution(Stochastic::DistributionTypes::DIST_FB_CF);
  TestDistribution(Stochastic::DistributionTypes::DIST_FB_HADOOP);
}

} // namespace Network