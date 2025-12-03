//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_FRUIT_H
#define PACMANGAME_FRUIT_H

#pragma once

#include "Entity.h"

class FruitModel : public Entity {
public:
    bool collected = false;
    void update(double dt) override {}
};

#endif //PACMANGAME_FRUIT_H