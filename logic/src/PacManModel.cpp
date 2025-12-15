//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/PacManModel.h"

#include <cmath>

#include "../include/logic/Stopwatch.h"


PacManModel::PacManModel() {
    // place start position and set speed
    x = 0.0; y = 0.6, speed = 3.0, direction = Direction::LEFT, desiredDirection = direction;
}


void PacManModel::setDirection(Direction d) { direction = d; }


void PacManModel::update(double dt) {
    // double dx = 0, dy = 0;
    // switch(direction) {
    //     case Direction::UP: dy = -1; break;
    //     case Direction::DOWN: dy = 1; break;
    //     case Direction::LEFT: dx = -1; break;
    //     case Direction::RIGHT: dx = 1; break;
    //     default: break;
    // }
    // // normalize step
    // if(dx != 0 || dy != 0) {
    //     double len = std::abs(dx) + std::abs(dy); // manhattan step
    //     x += (dx/len) * speed * dt;
    //     y += (dy/len) * speed * dt;
    // }
}


void PacManModel::reset() {
    x = 0.0; y = 0.6; direction = Direction::NONE;
}


void PacManModel::loseLife() { --lives; }