//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/PacManModel.h"
#include <cmath>
#include "../include/logic/Stopwatch.h"


PacManModel::PacManModel() {
    x = 0.0;
    y = 0.6;
    speed = 3.0;
}

void PacManModel::setDirection(Direction d) {
    direction = d;
}

void PacManModel::update(double dt) {
    // Nothing here, movement handled in World
}

void PacManModel::reset() {
    x = 0.0; y = 0.6; direction = Direction::NONE; desiredDirection = direction;
}



