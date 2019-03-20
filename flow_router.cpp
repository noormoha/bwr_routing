// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>
#include <queue>
#include <limits>

#include "flow_router.hpp"

namespace Network {

namespace {

void PathsPrint(Path* p) {
  for(Edge* const edge : p->GetEdges()) {
    cout << "[" << edge->GetSrc()->GetID() << "-" << edge->GetDst()->GetID() << "] ";
  }
  cout << endl;
}

}

FlowRouter::~FlowRouter() {
	// Release memory for completed flows.
	for(auto& flow_pair : flow_completion_times_) {
		for(Path* const path : flow_pair.first->GetPaths()) {
			delete path;
		}
		delete flow_pair.first;
	}
	// Release memory for incomplete flows.
	for(pair<Path* const, Flow*>& pair : paths_map_) {
		delete pair.first;
	}
	for(pair<int const, Flow*>& pair : flows_map_) {
		delete pair.second;
	}
}

double FlowRouter::getEpoch() {
	return time_;
}

// Compute transmission rates per path per flow and then update progress by one timeslot.
// This implements max-min fairness: (traffic is shifted to shorter paths in multipath mode)
unordered_map<Path*, double> FlowRouter::NextSlot() {
	// Next timeslot.
	time_ += TIMESLOT_DURATION;
	// Lookup tables.
	unordered_map<Edge*, unordered_map<Flow*, double> > edge_flow_utilization;
	unordered_map<Edge*, unordered_map<Flow*, unordered_map<Path*, double> > > edge_flow_path_allocated;
	unordered_map<Edge*, unordered_set<Flow*> > edge_flow_map;
	// Add all paths and order them by length.
	using Comparator = function<bool(Path* const, Path* const)>;
	map<Path*, Flow*, Comparator> ordered_flow_paths_by_length(
		[](Path* const p1, Path* const p2) {
			return (p1->GetEdges().size() < p2->GetEdges().size()) || 
				((p1->GetEdges().size() == p2->GetEdges().size()) && (p1 < p2));
		});
	for(auto& pair : paths_map_) {
		Path* const path = pair.first;
		Flow* const flow = pair.second;
		ordered_flow_paths_by_length.insert(make_pair(path, flow));
	}
	for(Edge* const edge : topo_->GetEdges()) {
		for(Path* const path : edges_map_[edge]) {
			Flow* const flow = paths_map_[path];
			assert(find(flow->GetPaths().begin(), flow->GetPaths().end(), path) != flow->GetPaths().end());
			edge_flow_path_allocated[edge][flow][path] = -1.0;
		}
	}
	// Compute fair shares.
	// Main loop of this algorithm, iterate over paths (groups of shortest paths among all existing).
	/////////////////////////////////////////////////////////////////////////////////////////////////
	for(;;) {
		// cout << "LOOP-------------------------------------------" << endl << endl;
		// Compute fair share per flow (not per path) per edge, find the edge with minimum fair share.
		tuple<Edge*, Flow*, double> flow_with_min_share = {NULL, NULL, numeric_limits<double>::max()};
		for(Edge* const edge : topo_->GetEdges()) {
			// cout << " [" << edge->GetSrc()->GetID() << ", " << edge->GetDst()->GetID() << "]:" << endl;
			unordered_set<Flow*> active_flows;
			unordered_map<Flow*, double> utilization;
			for(auto& pair : edge_flow_path_allocated[edge]) {
				Flow* const flow = pair.first;
				utilization[flow] = 0.0;
				for(auto& ppair : pair.second) {
					Path* const path = ppair.first;
					const double allocated = ppair.second;
					if(allocated > 0) {
						// cout << "Allocated: " << allocated << " --> ";
						// PathsPrint(path);
						utilization[flow] += allocated;
					}
					if(allocated < -0.5) {
						active_flows.insert(flow);
					}
				}
			}
			if(active_flows.empty()) {
				continue;
			}
			double remaining_capacity = edge->GetCap();
			for(auto& pair : utilization) {
				// cout << "Flow: [" << 
				// 	pair.first->GetSrc()->GetID() << ", " << pair.first->GetDst()->GetID() << "] " <<
				// 	pair.second << endl;
				if(active_flows.find(pair.first) == active_flows.end()) {
					remaining_capacity -= pair.second;
				}
			}
			assert(remaining_capacity > -1E-6);
			if(remaining_capacity < 1E-6) {
				continue;
			}
			const double edge_fair_share = remaining_capacity / active_flows.size();
			for(Flow* const flow : active_flows) {
				// cout << "Share: " << edge_fair_share << " Utilization: " << utilization[flow] << endl;
				assert(edge_fair_share - utilization[flow] > -1E-6);
				double flow_fair_share = min(
					max(edge_fair_share - utilization[flow], 0.0), 
						flow->GetRemainingSize() / TIMESLOT_DURATION);
				if(get<2>(flow_with_min_share) > flow_fair_share) {
					get<0>(flow_with_min_share) = edge;
					get<1>(flow_with_min_share) = flow;
					get<2>(flow_with_min_share) = flow_fair_share;
				}
			}
		}
		// Any edge with minimum fair share?
		if(get<0>(flow_with_min_share) == NULL) {
			break;
		}
		// Now distribute the capacity across paths by length (shortest paths get the highest rates).
		Edge* const min_edge = get<0>(flow_with_min_share);
		Flow* const min_flow = get<1>(flow_with_min_share);
		const double min_share = get<2>(flow_with_min_share);
		// Extract min-hop paths for this edge/flow pair.
		vector<Path*> min_paths, longer_paths;
		for(const pair<Path* const, Flow*> pair : ordered_flow_paths_by_length) {
			Path* const path = pair.first;
			Flow* const flow = pair.second;
			if(flow == min_flow) {
				if(path->GetEdgesSet().find(min_edge) != path->GetEdgesSet().end()) {
					if(edge_flow_path_allocated[min_edge][min_flow][path] < -0.5) {
						if(min_paths.empty() || 
							(!min_paths.empty() && (min_paths.back()->GetEdges().size() == path->GetEdges().size()))) {
							min_paths.push_back(path);
						} else if(!min_paths.empty() && (min_paths.back()->GetEdges().size() < path->GetEdges().size())) {
							longer_paths.push_back(path);
						}
					}
				}
			}
		}
		// Split the fair share equally across all min-hop paths.
		// cout << endl << "From here................ " << min_edge << " " << min_flow << " " << min_share << " " << min_paths.size() << endl;
		for(Path* const min_path : min_paths) {
			// cout << "Shortest Paths: ";
			// PathsPrint(min_path);
			assert(paths_map_[min_path] == min_flow);
			assert(ordered_flow_paths_by_length.find(min_path) != ordered_flow_paths_by_length.end());
			ordered_flow_paths_by_length.erase(min_path);			
			const double path_share = min_share / min_paths.size();
			for(Edge* const edge : min_path->GetEdges()) {
				edge_flow_path_allocated[edge][min_flow][min_path] = path_share;
			}
		}
		for(Path* const long_path : longer_paths) {
			// cout << "Lonegr Paths: ";
			// PathsPrint(long_path);
			ordered_flow_paths_by_length.erase(long_path);
			for(Edge* const edge : long_path->GetEdges()) {
				edge_flow_path_allocated[edge][min_flow][long_path] = 0.0;
			}
		}
	}
	// Extract allocated rate per path from the detailed per edge allocations.
	unordered_map<Path*, double> path_allocated_rate;
	for(Edge* const edge : topo_->GetEdges()) {
		for(auto& pair : edge_flow_path_allocated[edge]) {
			Flow* const flow = pair.first;
			for(auto& ppair : pair.second) {
				Path* const path = ppair.first;
				const double allocated = ppair.second;
				if(allocated > 0) {
					if(path_allocated_rate.find(path) != path_allocated_rate.end()) {
						assert(path_allocated_rate[path] == allocated);
						continue;
					}
					path_allocated_rate[path] = allocated;
				}
			}
		}
	}
	// Now update the remaining bytes for all flows given the rates per path.
	unordered_map<Edge*, double> utilization;
	for(pair<Path* const, double> allocation : path_allocated_rate) {
		for(Edge* const edge : allocation.first->GetEdges()) {
			utilization[edge] += allocation.second;
		}
		Flow* const flow = paths_map_[allocation.first];
		flow->AddCompleted(allocation.second * TIMESLOT_DURATION);
		// cout << "From " << flow->GetID() << ", Completed: " << (allocation.second * TIMESLOT_DURATION) << endl;
	}
	// Verify link utilization is valid.
	for(Edge* const edge : topo_->GetEdges()) {
		if(utilization[edge] >= edge->GetCap() + 1E-6) {
			cout << edge->GetCap() << " -> " << utilization[edge] << endl;
			for(Path* const path : edges_map_[edge]) {
				PathsPrint(path);
			}
		}
		assert(utilization[edge] < edge->GetCap() + 1E-6);
		assert(utilization[edge] > -1E-6);
		edge_utilization_[edge] = (utilization[edge] / edge->GetCap());
	}
	// Delete all completed flows.
	vector<int> completed_flows;
	for(pair<const int, Flow*>& flow_pair : flows_map_) {
		Flow* const flow = flow_pair.second;
		if(flow->GetRemainingSize() < 1E-6) {
			completed_flows.push_back(flow_pair.first);
			flow_completion_times_[flow] = time_;
			for(Path* const path : flow->GetPaths()) {
				paths_map_.erase(path);
				for(Edge* const edge : path->GetEdges()) {
					auto& paths = edges_map_[edge];
					auto it = find(paths.begin(), paths.end(), path);
					assert(it != paths.end());
					paths.erase(it);
				}
			}
		}
	}
	// Erase all completed flows.
	for(int flow_id : completed_flows) {
		flows_map_.erase(flow_id);
	}
	// Verify consistency of data.
	VerifyConsistency();
	// Return the path rates.
	return path_allocated_rate;
}

vector<double> FlowRouter::GetCompletionTimes() {
	vector<double> flows;
	for(auto& flow_pair : flow_completion_times_) {
		flows.push_back(flow_pair.second);
	}
	return flows;
}

int FlowRouter::getRemainingFlows() {
	return flows_map_.size();
}

void FlowRouter::VerifyConsistency() {
	for(auto& pair : flows_map_) {
		assert(pair.first == pair.second->GetID());
	}
	unordered_map<Path*, unordered_set<Edge*> > path_to_edges;
	for(auto& pair : edges_map_) {
		unordered_set<Path*> paths;
		for(auto path : pair.second) {
			paths.insert(path);
			path_to_edges[path].insert(pair.first);
			assert(find(path->GetEdges().begin(), path->GetEdges().end(), pair.first) != path->GetEdges().end());
		}
		assert(paths.size() == pair.second.size());
	}
	for(auto& pair : paths_map_) {
		assert(pair.first->GetEdgesSet().size() == path_to_edges[pair.first].size());
		for(auto edge : pair.first->GetEdgesSet()) {
			assert(path_to_edges[pair.first].find(edge) != path_to_edges[pair.first].end());
		}
		for(auto edge : path_to_edges[pair.first]) {
			assert(pair.first->GetEdgesSet().find(edge) != pair.first->GetEdgesSet().end());
		}
	}
}

double FlowRouter::GetTotalRemainingDemand() {
	double total_vol = 0.0;
	for(auto& pair : flows_map_) {
		total_vol += pair.second->GetRemainingSize();
	}
	return total_vol;
}

double FlowRouter::GetEdgeUtilization(Edge* const edge) const {
	return (edge_utilization_.find(edge) == edge_utilization_.end()) ?
			0.0 : edge_utilization_.find(edge)->second;
}

} // namespace Network
