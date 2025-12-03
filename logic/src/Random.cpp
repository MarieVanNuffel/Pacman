//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/Random.h"
#include <chrono>


logic::Random::Random() {
    rng.seed(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
}


logic::Random&logic::Random::instance() {
    static Random r;
    return r;
}


int logic::Random::nextInt(int a, int b) {
    std::uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}


double logic::Random::nextDouble(double a, double b) {
    std::uniform_real_distribution<double> dist(a, b);
    return dist(rng);
}

