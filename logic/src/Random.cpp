//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/Random.h"

namespace logic {

Random::Random() {
    std::random_device rd;
    engine_ = std::mt19937(rd());
}

Random& Random::instance() {
    static Random inst;
    return inst;
}

int Random::uniformInt(int a, int b) {
    std::uniform_int_distribution<int> dist(a, b);
    return dist(engine_);
}

double Random::uniformReal(double a, double b) {
    std::uniform_real_distribution<double> dist(a, b);
    return dist(engine_);
}

}
