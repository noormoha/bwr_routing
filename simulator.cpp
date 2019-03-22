// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <sstream>

#include "simulator.hpp"

using namespace std;

namespace Network {

constexpr bool REPORT_ALL_PERCENTILES = false;

Logger::Logger(string filename) : filename_(filename), file_(fopen(filename.c_str(), "w")) {
	assert(file_ != NULL);
	string data = "stats = [\r\n";
	fputs(data.c_str(), file_);
	fflush(file_);
}

Logger::~Logger() {
	Close();
}

void Logger::Log(const Scenario& scenario, const int router_id, vector<double> completion_times) {
	// Write a row in the output matrix log file.
	assert(!completion_times.empty());
	sort(completion_times.begin(), completion_times.end());
	stringstream ss;
	ss << 
		scenario.K << ", " << 
		scenario.lambda << ", " << 
		scenario.mu << ", " << 
		static_cast<int>(scenario.dist_type) << ", " << 
		scenario.sim_duration << ", " <<
		hash<string>()(scenario.topo->GetName()) << ", " <<
		router_id << ", ";
	if(REPORT_ALL_PERCENTILES) {
		for(double completion_time : completion_times) {
			ss << completion_time << ", ";
		}
	} else {
		ss << completion_times.back() << ", ";
		ss << completion_times[static_cast<int>(completion_times.size() * 0.99)] << ", ";
		ss << completion_times[static_cast<int>(completion_times.size() * 0.95)] << ", ";
	}
	ss << completion_times[static_cast<int>(completion_times.size() * 0.5)] << ";";
	string data = ss.str() + "\r\n";
	cout << "Logging " << data.size() << " bytes into " << filename_ << endl << endl;
	int error = fputs(data.c_str(), file_);
	assert(error >= 0);
	error = fflush(file_);
	assert(error == 0);
}

void Logger::Close() {
	// Write the footer and close the file.
	string data = "]\r\n";
	fputs(data.c_str(), file_);
	fflush(file_);
	fclose(file_);
}

long GenerateTimestamp() {
	auto epoch = chrono::system_clock::now().time_since_epoch();
    return chrono::duration_cast<chrono::milliseconds>(epoch).count();
}

vector<tuple<double, double, int, int>> GenerateTraffic(Scenario& scenario) {
	vector<tuple<double, double, int, int>> traffic;
	Stochastic dist(scenario.lambda, scenario.mu, scenario.dist_type);
	pair<double, double> flow;
	int nodes = scenario.topo->GetNodes().size();
	vector<pair<int, int>> src_dst_pairs;
	for(int src = 0; src < nodes; src++) {
		for(int dst = src+1; dst < nodes; dst++) {
			src_dst_pairs.push_back(make_pair(src, dst));
		}
	}
	srand(GenerateTimestamp());
	while((flow = dist.nextSample()).first < scenario.sim_duration) {
		int src_dst_index = rand() % src_dst_pairs.size();
		auto& src_dst_pair = src_dst_pairs[src_dst_index];
		traffic.push_back(make_tuple(flow.first, flow.second, src_dst_pair.first, src_dst_pair.second));
		// cout << flow.first << ", " << flow.second << ", " << src_dst_pair.first << ", " << src_dst_pair.second << endl;
	}
	return traffic;
}

void RunSimulations(vector<Scenario> scenarios, vector<RouterFactory::RouterType> routers) {
	Logger logger("stats/matrix_" + to_string(GenerateTimestamp()) + ".m");
	// Iterate over scenarios and run the routers on each scenario.
	// Write the output for each scenario.
	for(Scenario& scenario : scenarios) {
		auto traffic = GenerateTraffic(scenario);
		for(RouterFactory::RouterType router_type : routers) {
			cout << "Starting router " << static_cast<int>(router_type) << " over " << traffic.size() << " flows..." << endl << endl;
			FlowRouter* router = RouterFactory::BuildRouter(router_type, scenario.K, scenario.topo);
			int index = 0, next_time = 0;
			while(index < traffic.size() || router->getRemainingFlows() > 0) {
				while(index < traffic.size() && get<0>(traffic[index]) < router->getEpoch()) {
					cout << index << " " << flush;
					router->PostFlow(Flow(index, 
						scenario.topo->GetNode(get<2>(traffic[index])), 
						scenario.topo->GetNode(get<3>(traffic[index])), 
						get<1>(traffic[index])));
					index++;
				}
				router->NextSlot();
				if(router->getEpoch() >= next_time) {
					cout.precision(5);
					cout << (next_time < 10 ? "" : "]") << endl << "[ " << static_cast<int>(router_type) << " ] Rem Flows: " << router->getRemainingFlows() << 
									"\tRemaining Vol: " << round(router->GetTotalRemainingDemand()) << 
									"\tElapsed: " << round(router->getEpoch()) << 
									"\tFlow Index: [ ";
					next_time += 10;
				}
			}
			cout << endl;
			logger.Log(scenario, static_cast<int>(router_type), router->GetCompletionTimes());
			delete router;
		}
	}
}

} // namespace Network
