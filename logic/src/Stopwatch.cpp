//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/Stopwatch.h"

namespace logic {
    Stopwatch& Stopwatch::instance() {
        static Stopwatch inst;  // lokaal static object voor singleton
        return inst;
    }

    Stopwatch::Stopwatch() {
        last = std::chrono::steady_clock::now();
    }

    double Stopwatch::tick() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = now - last; // verschil berekenen
        last = now;
        return diff.count();
    }
}