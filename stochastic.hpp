// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#ifndef STOCHASTIC_HPP
#define STOCHASTIC_HPP

#include <cassert>
#include <chrono>
#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace Network {

class Stochastic {
public:
	// Different flow size distributions considered.
	enum class DistributionTypes {
        DIST_EXPONENTIAL,
        DIST_PARETO,
        DIST_FB_CF,
        DIST_FB_HADOOP,
    };

	Stochastic(double lambda, double mu, DistributionTypes job_size_dist) : 
		lambda_(lambda), mu_(mu), job_size_dist_(job_size_dist), t_fraction_(0.0) {
		    auto epoch = std::chrono::system_clock::now().time_since_epoch();
		    srand(std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count());
		}

	pair<double, double> nextSample(); // Returns a pair<arrival, volume> that is a sampled flow.

private:
	// Parameters used to generate traffic.
	const double lambda_, mu_;
	const DistributionTypes job_size_dist_;

	// Parameters used to generate samples.
	double t_fraction_;

	// Generate a double uniformly distributed in [0, 1].
    double genRand();

    // Generate a sample distributed according to any of the following distributions.
    double expGen();    
    double paretoGen();    
    double fbcf();    
    double fbhadoop();
};

} // namespace Network

#endif // STOCHASTIC_HPP