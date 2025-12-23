//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_IENTITYFACTORY_H
#define PACMANGAME_IENTITYFACTORY_H

#pragma once


#include <memory>

#include "GhostDoorModel.h"


class Entity;
class PacManModel;
class GhostModel;
class CoinModel;
class FruitModel;
class EntityView;
class Observer;

class IEntityFactory {
public:
    virtual ~IEntityFactory() = default;

    virtual std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createPacmanView(PacManModel* model) = 0;

    virtual std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createGhostView(GhostModel* model) = 0;

    virtual std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createCoinView(CoinModel* model) = 0;

    virtual std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createFruitView(FruitModel* model) = 0;

    virtual std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
    createGhostDoorView(GhostDoorModel* model) = 0;
};




#endif //PACMANGAME_IENTITYFACTORY_H