//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/Stopwatch.h"

namespace logic {

Stopwatch::Stopwatch() {
    last_ = std::chrono::steady_clock::now();
    dt_ = 0;
}

Stopwatch& Stopwatch::instance() {
    static Stopwatch inst;
    return inst;
}

void Stopwatch::tick() {
    auto now = std::chrono::steady_clock::now();
    dt_ = std::chrono::duration<double>(now - last_).count();
    last_ = now;
}

double Stopwatch::deltaTime() const {
    return dt_;
}

}
