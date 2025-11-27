//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_RANDOM_H
#define PACMAN_RANDOM_H


#pragma once
#include <random>

namespace logic {

    class Random {
    public:
        static Random& instance();

        int uniformInt(int a, int b);
        double uniformReal(double a, double b);

    private:
        Random();
        std::mt19937 engine_;
    };

}


#endif //PACMAN_RANDOM_H