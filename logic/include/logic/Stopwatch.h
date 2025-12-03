//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STOPWATCH_H
#define PACMAN_STOPWATCH_H


#pragma once
#include <chrono>

namespace logic {

#pragma once


#include <chrono>


    class Stopwatch {
    public:
        static Stopwatch& instance();
        double tick(); // returns delta time in seconds since last tick


    private:
        Stopwatch();
        std::chrono::steady_clock::time_point last;
    };


}


#endif //PACMAN_STOPWATCH_H