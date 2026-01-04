//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_FRUIT_H
#define PACMANGAME_FRUIT_H

#pragma once

#include "Entity.h"
#include "Subject.h"

namespace logic {

    /**
     * @brief Model voor een fruit entity
     */
    class FruitModel : public Entity, public Subject {
    public:
        FruitModel() = default;
        ~FruitModel() override = default;

        /**
         * @brief fruit als gegeten en notify observers
         */
        void collect();

        /**
         * @brief Update functie
         *
         * Fruit is statisch, dus doet niets
         * @param dt Delta time
         */
        void update(double dt) override;

        /**
         * @brief Reset fruit originele staat, notify observers
         */
        void reset();

        bool collected = false;
    };
}

#endif //PACMANGAME_FRUIT_H