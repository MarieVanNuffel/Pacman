//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/CoinModel.h"

void CoinModel::collect() {
    if (!collected) {
        collected = true;
        notify(1); // 1 = coin collected
    }
}

void CoinModel::update(double /*dt*/) {
    // coins zijn statisch — geen update nodig
}

void CoinModel::reset() {
    collected = false;
    notify(0); // zodat view weer zichtbaar wordt
}