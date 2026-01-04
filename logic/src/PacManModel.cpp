//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/PacManModel.h"
#include "logic/Events.h"

namespace logic {
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

    void PacManModel::die() {
        notify(static_cast<int>(logic::Event::PACMAN_DIED));
    }

    void PacManModel::revive() {
        notify(static_cast<int>(logic::Event::PACMAN_REVIVED));
    }
}
