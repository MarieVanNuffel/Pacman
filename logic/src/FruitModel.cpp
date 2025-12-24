//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "../include/logic/FruitModel.h"

void FruitModel::collect() {
    if (!collected) {
        collected = true;
        notify(2); // 2 = fruit collected
    }
}

void FruitModel::update(double /*dt*/) {
    // Fruits are static by default; no per-frame logic here.
}

void FruitModel::reset() {
    collected = false;
    notify(0);
}