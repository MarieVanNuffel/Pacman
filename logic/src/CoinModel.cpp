//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/CoinModel.h"
#include "logic/Events.h"

namespace logic {
    void CoinModel::collect() {
        if (!collected) {
            collected = true;
            notify(static_cast<int>(logic::Event::COIN_COLLECTED));
        }
    }

    void CoinModel::reset() {
        collected = false;
        notify(static_cast<int>(logic::Event::COIN_RESPAWN));
    }

    void CoinModel::update(double dt) {
        // statisch, dus niet nodig
    }
}