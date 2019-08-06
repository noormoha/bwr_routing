// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <cstdio>

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "flow_router.hpp"
#include "router_factory.hpp"
#include "stochastic.hpp"

using namespace std;

namespace Network {

struct Scenario {
  // Traffic parameters specified for Stochastic object to generate flows.
  double lambda;
  double mu;
  Stochastic::DistributionTypes dist_type;
  double sim_duration;
  // Topology supplied to the FlowRouter object.
  Topology* topo;

  Scenario(double lambda_, double mu_, Stochastic::DistributionTypes dist_type_, double sim_duration_, Topology* topo_) : 
    lambda(lambda_), mu(mu_), dist_type(dist_type_), sim_duration(sim_duration_), topo(topo_) {}
};

class Logger {
public:
  explicit Logger(string filename);
  ~Logger();
  void Log(const Scenario& scenario, const int router_id, vector<double> completion_times);
  void Close();
private:
  const string filename_;
  FILE* const file_;
};

long GenerateTimestamp();

vector<tuple<double, double, int, int>> GenerateTraffic(Scenario& scenario);

void RunSimulations(vector<Scenario> scenarios, vector<RouterFactory::RouterType> routers);

} // namespace Network

#endif // SIMULATOR_HPP