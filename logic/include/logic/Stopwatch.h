//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STOPWATCH_H
#define PACMAN_STOPWATCH_H


#pragma once
#include <chrono>

namespace logic {

    class Stopwatch {
    public:
        static Stopwatch& instance();
        void tick(); // update intern tijdsverschil
        double deltaTime() const; // in seconden

    private:
        Stopwatch();
        std::chrono::steady_clock::time_point last_; // last time
        double dt_; // delta time
    };

}


#endif //PACMAN_STOPWATCH_H