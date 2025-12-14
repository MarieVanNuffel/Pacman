//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_FRUIT_H
#define PACMANGAME_FRUIT_H

#pragma once

#include "Entity.h"
#include "Subject.h"

class FruitModel : public Entity, public Subject {
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

#endif //PACMANGAME_FRUIT_H