//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_FRUIT_H
#define PACMANGAME_FRUIT_H

#pragma once

#include "Entity.h"
#include "Subject.h"

namespace logic {
    class FruitModel : public Entity, public Subject {
    public:
        FruitModel() = default;
        ~FruitModel() override = default;

        // mark collected and notify observers (event == 2 for fruit)
        void collect();

        // fruit is static by default; implement update as no-op so class is not abstract
        void update(double dt) override;

        void reset();

        bool collected = false;
    };
}

#endif //PACMANGAME_FRUIT_H