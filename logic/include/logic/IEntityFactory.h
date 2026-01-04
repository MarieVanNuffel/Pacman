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
    class IEntityFactory {
    public:
        virtual ~IEntityFactory() = default;

        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createPacmanView(std::shared_ptr<PacManModel> model) = 0;

        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createGhostView(std::shared_ptr<GhostModel> model) = 0;

        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createCoinView(std::shared_ptr<CoinModel> model) = 0;

        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createFruitView(std::shared_ptr<FruitModel> model) = 0;

        virtual std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<Observer>>
        createGhostDoorView(std::shared_ptr<GhostDoorModel> model) = 0;
    };
}



#endif //PACMANGAME_IENTITYFACTORY_H