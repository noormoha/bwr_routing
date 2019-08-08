// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <iostream>
#include <queue>
#include <functional>
#include <unordered_set>

#include <execinfo.h>
#include <unistd.h>
#include <signal.h>

#include "bwr_router.hpp"
#include "router_factory.hpp"
#include "tests.hpp"
#include "flow_router.hpp"
#include "simulator.hpp"
#include "topologies.hpp"

using namespace std;

void PrintStackTrace(int sigmessage) {
	const int lines = 20;
	void* array[lines];
	const int size = backtrace(array, lines);
	fprintf(stderr, "------------------------- Stack Trace (signal %d) -------------------------\n", sigmessage);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

namespace Network {
vector<RouterFactory::RouterType> ListRouters() {
	return { 
		RouterFactory::RouterType::BWR_ROUTER_BWRH,
	    RouterFactory::RouterType::BWR_ROUTER_BWRHF,
	    RouterFactory::RouterType::SHORTEST_PATH_ROUTER_BY_HOPS,
	    RouterFactory::RouterType::SHORTEST_PATH_ROUTER_BY_INVERSE_CAPACITY,
	    RouterFactory::RouterType::UTILIZATION_ROUTER,
	};
}

vector<Scenario> BuildScenarios() {
	return {
		// Each row is: {double lambda_, double mu_, Stochastic::DistributionTypes dist_type_, double sim_duration_, Topology* topo_}
		// {1, 1.0, 0.1, Stochastic::DistributionTypes::DIST_EXPONENTIAL, 500.0, BuildTopologyGSCALE()},
		{0.2, 0.2, Stochastic::DistributionTypes::DIST_EXPONENTIAL, 400.0, BuildTopologyUNINETT2011()},
	};
}
} // namespace Network

int main() {
	// Run all tests, crash if anything fails.
	signal(SIGABRT, PrintStackTrace);
	signal(SIGSEGV, PrintStackTrace);
	signal(SIGILL,  PrintStackTrace);
	signal(SIGFPE,  PrintStackTrace);

	// TODO: maybe use Google Test framework to make tests more readable.
	// This runs some basic tests to make sure nothing is broken.
	Network::RunAllTests();

	// Run actual simulations.
	Network::RunSimulations(Network::BuildScenarios(), Network::ListRouters());
}