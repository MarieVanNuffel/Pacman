//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/FruitModel.h"
#include "logic/Events.h"

namespace logic {
    void FruitModel::collect() {
        if (!collected) {
            collected = true;
            notify(static_cast<int>(logic::Event::FRUIT_EATEN)); // stuur event
        }
    }

    void FruitModel::update(double /*dt*/) {
        // statisch, dus doet niets
    }

    void FruitModel::reset() {
        collected = false;
        notify(static_cast<int>(logic::Event::FRUIT_RESPAWN)); // stuur event
    }
}