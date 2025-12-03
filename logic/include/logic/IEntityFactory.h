//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_IENTITYFACTORY_H
#define PACMANGAME_IENTITYFACTORY_H

#pragma once


#include <memory>


class Entity;
class PacManModel;
class GhostModel;
class CoinModel;
class FruitModel;


// Forward declare view-side type to avoid linking SFML in logic
class EntityView; // the view typedef lives in view part


class IEntityFactory {
public:
    virtual ~IEntityFactory() = default;


    // These return nullptr in logic, concrete factory in view returns valid view pointers
    virtual std::shared_ptr<EntityView> createPacmanView(PacManModel* model) = 0;
    virtual std::shared_ptr<EntityView> createGhostView(GhostModel* model) = 0;
    virtual std::shared_ptr<EntityView> createCoinView(CoinModel* model) = 0;
    virtual std::shared_ptr<EntityView> createFruitView(FruitModel* model) = 0;
};



#endif //PACMANGAME_IENTITYFACTORY_H