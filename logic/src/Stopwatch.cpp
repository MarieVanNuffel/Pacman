//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/Stopwatch.h"

namespace logic {

    Stopwatch::Stopwatch() : last(std::chrono::steady_clock::now()) {}


    Stopwatch& Stopwatch::instance() {
        static Stopwatch inst;
        return inst;
    }


    double Stopwatch::tick() {
        using namespace std::chrono;
        auto now = steady_clock::now();
        duration<double> diff = now - last;
        last = now;
        return diff.count();
    }

}
