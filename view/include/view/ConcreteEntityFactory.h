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
#include "CoinView.h"
#include "FruitView.h"
#include "GhostDoorView.h"

namespace logic {
    class PacManModel;
    class GhostModel;
    class CoinModel;
    class FruitModel;
    class GhostDoorModel;
    class Observer;
}

class ConcreteEntityFactory : public logic::IEntityFactory {
public:
    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    createPacmanView(logic::PacManModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    createGhostView(logic::GhostModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    createCoinView(logic::CoinModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    createFruitView(logic::FruitModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    createGhostDoorView(logic::GhostDoorModel* m) override;
};




#endif //PACMANGAME_CONCRETEENTITYFACTORY_H