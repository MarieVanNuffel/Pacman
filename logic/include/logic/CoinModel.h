//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_COIN_H
#define PACMANGAME_COIN_H

#pragma once

#include "Entity.h"

class CoinModel : public Entity {
public:
    bool collected = false;
    void update(double dt) override {}
};



#endif //PACMANGAME_COIN_H