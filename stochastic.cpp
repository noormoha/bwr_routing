// Author: Mohammad Noormohammadpour
// Copyright 2019
// Can be distributed under MIT license (https://opensource.org/licenses/MIT)

#include <cstdlib>

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "stochastic.hpp"

using namespace std;

namespace Network {

double Stochastic::genRand() {
	return (static_cast<double>(rand() + 1) / RAND_MAX);
}

double Stochastic::expGen() {
    double rnd = genRand();
    return (-log(rnd) / mu_);
}

double Stochastic::paretoGen() {        
    double xm = 0.1;
    double alpha = 10.0;
    return (xm / pow(genRand(), 1.0 / alpha)) * 9.0 / mu_;
}

double Stochastic::fbcf() {
    double sample;
    double uniform = genRand();
    if(uniform < 0.1) {
        sample = 0.1;
    } else if(uniform < 0.4) {
        sample = 1;
    } else if(uniform < 0.8) {
        sample = 10;
    } else if(uniform < 0.91) {
        sample = 100;
    } else if(uniform < 0.93) {
        sample = 1000;
    } else {
        sample = 10000;
    }
    return sample / (735.31 * mu_);
}

double Stochastic::fbhadoop() {
    double sample;
    double uniform = genRand();
    if(uniform < 0.8) {
        sample = 1;
    } else if(uniform < 0.95) {
        sample = 10;
    } else if(uniform < 0.98) {
        sample = 100;
    } else {
        sample = 1000;
    }
    return sample / (25.3 * mu_);
}

pair<double, double> Stochastic::nextSample() {
    // For Poisson arrivals, inter-arrival times are exponential so we increase it like follows.
    t_fraction_ += (-log(genRand()) / lambda_);

    double arrival = t_fraction_;

    double volume;
    switch(job_size_dist_) {
        case DistributionTypes::DIST_EXPONENTIAL:
            volume = expGen();
            break;
        case DistributionTypes::DIST_PARETO:
            volume = paretoGen();
            break;
        case DistributionTypes::DIST_FB_CF:
            volume = fbcf();
            break;
        case DistributionTypes::DIST_FB_HADOOP:
            volume = fbhadoop();
            break;
        default:
            assert(false);
    }
    
    volume = min(volume, 1000.0 / mu_);
    
    return {arrival /* arrival */, volume /* volume */};
}

} // namespace Network
