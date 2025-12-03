//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_RANDOM_H
#define PACMAN_RANDOM_H


#pragma once
#include <random>

namespace logic {

#pragma once


#include <random>


    class Random {
    public:
        static Random& instance();
        int nextInt(int a, int b);
        double nextDouble(double a, double b);
    private:
        Random();
        std::mt19937 rng;
    };


}


#endif //PACMAN_RANDOM_H