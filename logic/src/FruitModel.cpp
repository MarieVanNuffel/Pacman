//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/FruitModel.h"
#include "logic/Events.h"

namespace logic {
    void FruitModel::collect() {
        if (!collected) {
            collected = true;
            notify(static_cast<int>(logic::Event::FRUIT_EATEN));
        }
    }

    void FruitModel::update(double /*dt*/) {
        // Fruits are static by default; no per-frame logic here.
    }

    void FruitModel::reset() {
        collected = false;
        notify(static_cast<int>(logic::Event::FRUIT_RESPAWN));
    }
}