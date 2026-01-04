//
// Created by Marie Van Nuffel on 14/12/2025.
//

#include "view/ConcreteEntityFactory.h"
#include "view/CoinView.h"
#include "view/FruitView.h"

// Pacman
std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createPacmanView(logic::PacManModel* m) {
    auto view = std::make_shared<PacmanView>(m);
    std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(view);
    return { view, obs };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createGhostView(logic::GhostModel* m) {
    auto view = std::make_shared<GhostView>(m);
    return { view, nullptr };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createCoinView(logic::CoinModel* m) {
    auto view = std::make_shared<CoinView>(m);
    std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(view);
    return { view, obs };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createFruitView(logic::FruitModel* m) {
    auto view = std::make_shared<FruitView>(m);
    std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(view);
    return { view, obs };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createGhostDoorView(logic::GhostDoorModel* m) {
    auto view = std::make_shared<GhostDoorView>(m);
    return { view, nullptr };
}