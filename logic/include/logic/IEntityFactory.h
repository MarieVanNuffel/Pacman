//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_IENTITYFACTORY_H
#define PACMANGAME_IENTITYFACTORY_H

#pragma once


#include <memory>
#include "GhostDoorModel.h"


namespace view { class EntityView; }

namespace logic {
    class Observer;
    class Entity;
    class PacManModel;
    class GhostModel;
    class CoinModel;
    class FruitModel;
}

namespace logic {

    /**
     * @brief Abstract factory voor entity views
     *
     * Maakt views en observers voor entities.
     * Wordt gebruikt door World.
     */
    class IEntityFactory {
    public:
        virtual ~IEntityFactory() = default;

        /**
         * @brief Maak pacman view + observer
         * @param model PacMan model
         * @return Pair van EntityView en Observer
         */
        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createPacmanView(std::shared_ptr<PacManModel> model) = 0;

        /**
         * @brief Maak ghost view + observer
         * @param model Ghost model
         * @return Pair van EntityView en Observer
         */
        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createGhostView(std::shared_ptr<GhostModel> model) = 0;

        /**
         * @brief Maak coin view + observer
         * @param model Coin model
         * @return Pair van EntityView en Observer
         */
        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createCoinView(std::shared_ptr<CoinModel> model) = 0;

        /**
         * @brief Maak fruit view + observer
         * @param model Fruit model
         * @return Pair van EntityView en Observer
         */
        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createFruitView(std::shared_ptr<FruitModel> model) = 0;

        /**
         * @brief Maak ghostdoor view + observer
         * @param model GhostDoor model
         * @return Pair van EntityView en Observer
         */
        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createGhostDoorView(std::shared_ptr<GhostDoorModel> model) = 0;
    };
}



#endif //PACMANGAME_IENTITYFACTORY_H