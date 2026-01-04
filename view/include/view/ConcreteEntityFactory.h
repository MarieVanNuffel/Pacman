//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CONCRETEENTITYFACTORY_H
#define PACMANGAME_CONCRETEENTITYFACTORY_H


#pragma once

#include <memory>
#include "logic/IEntityFactory.h"
#include "PacManView.h"
#include "GhostView.h"
#include "GhostDoorView.h"

namespace logic {
    class PacManModel;
    class GhostModel;
    class CoinModel;
    class FruitModel;
    class GhostDoorModel;
    class Observer;
}

namespace view {
    class ConcreteEntityFactory : public logic::IEntityFactory {
    public:
        std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
        createPacmanView(std::shared_ptr<logic::PacManModel> m) override;

        std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
        createGhostView(std::shared_ptr<logic::GhostModel> m) override;

        std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
        createCoinView(std::shared_ptr<logic::CoinModel> m) override;

        std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
        createFruitView(std::shared_ptr<logic::FruitModel> m) override;

        std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
        createGhostDoorView(std::shared_ptr<logic::GhostDoorModel> m) override;
    };
}



#endif //PACMANGAME_CONCRETEENTITYFACTORY_H