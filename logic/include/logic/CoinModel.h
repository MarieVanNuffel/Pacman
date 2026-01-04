//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_COIN_H
#define PACMANGAME_COIN_H

#pragma once

#include "Entity.h"
#include "Subject.h"

namespace logic {
    class CoinModel : public Entity, public Subject {
    public:
        CoinModel() = default;
        ~CoinModel() override = default;

        // mark collected and notify observers
        void collect();

        // coin is static; implement update as no-op so class is not abstract
        void update(double dt) override;

        void reset();

        bool collected = false;
    };

}





#endif //PACMANGAME_COIN_H