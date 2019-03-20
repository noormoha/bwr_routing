// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef TESTS_HPP
#define TESTS_HPP

#include <cassert>
#include <functional>

#include "topology.hpp"
#include "bwr_router.hpp"
#include "shortest_path_router.hpp"
#include "utilization_router.hpp"
#include "stochastic.hpp"

using namespace std;

namespace Network {

class BWRRouterTest : public BWRRouter {
public:
  BWRRouterTest(int K, Topology* topo, BWRRouter::TECHNIQUE tech) : BWRRouter(K, topo, tech) {}
  void RunTests();
};

class ShortestPathRouterTest : public ShortestPathRouter {
public:
  ShortestPathRouterTest(int K, Topology* topo, ShortestPathRouter::TECHNIQUE tech) : ShortestPathRouter(K, topo, tech) {}
  void RunTests();
};

class UtilizationRouterTest : public UtilizationRouter {
public:
  UtilizationRouterTest(int K, Topology* topo) : UtilizationRouter(K, topo) {}
  void RunTests();
};

bool PathsEqual(Path* p1, Path* p2);

void PathsPrint(Path* p);

Topology* BuildTopology();

void TestDistribution(Stochastic::DistributionTypes dist_type);

void RunAllTests();

} // namespace Network

#endif // TESTS_HPP