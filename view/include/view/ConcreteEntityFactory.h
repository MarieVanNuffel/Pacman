//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CONCRETEENTITYFACTORY_H
#define PACMANGAME_CONCRETEENTITYFACTORY_H


#pragma once

#include "logic/IEntityFactory.h"
#include "PacManView.h"
#include "GhostView.h"
#include "CoinView.h"
#include "FruitView.h"

class ConcreteEntityFactory : public IEntityFactory {
public:
    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createPacmanView(PacManModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createGhostView(GhostModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createCoinView(CoinModel* m) override;

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createFruitView(FruitModel* m) override;
};



#endif //PACMANGAME_CONCRETEENTITYFACTORY_H