//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_COIN_H
#define PACMANGAME_COIN_H

#pragma once

#include "Entity.h"
#include "Subject.h"

class CoinModel : public Entity, public Subject {
public:
    bool collected = false;

    void collect() {
        if(!collected) {
            collected = true;
            notify(1); // 1 = coin collected, kan enum later
        }
    }

    void update(double dt) override {}
};




#endif //PACMANGAME_COIN_H