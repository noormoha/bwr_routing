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
  BWRRouterTest(Topology* topo, BWRRouter::TECHNIQUE tech) : BWRRouter(topo, tech) {}
  void RunTests();
};

class ShortestPathRouterTest : public ShortestPathRouter {
public:
  ShortestPathRouterTest(Topology* topo, ShortestPathRouter::TECHNIQUE tech) : ShortestPathRouter(topo, tech) {}
  void RunTests();
};

class UtilizationRouterTest : public UtilizationRouter {
public:
  UtilizationRouterTest(Topology* topo) : UtilizationRouter(topo) {}
  void RunTests();
};

bool PathsEqual(Path* p1, Path* p2);

void PathsPrint(Path* p);

Topology* BuildTopology();

void TestDistribution(Stochastic::DistributionTypes dist_type);

void RunAllTests();

} // namespace Network

#endif // TESTS_HPP