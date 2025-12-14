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
    std::shared_ptr<EntityView> createPacmanView(PacManModel* m) override {
        return std::make_shared<PacmanView>(m);
    }

    std::shared_ptr<EntityView> createGhostView(GhostModel* m) override {
        return std::make_shared<GhostView>(m);
    }

    std::shared_ptr<EntityView> createCoinView(CoinModel* m) override {
        auto view = std::make_shared<CoinView>(m);
        return view;
    }

    std::shared_ptr<EntityView> createFruitView(FruitModel* m) override {
        return std::make_shared<FruitView>(m);
    }
};




#endif //PACMANGAME_CONCRETEENTITYFACTORY_H