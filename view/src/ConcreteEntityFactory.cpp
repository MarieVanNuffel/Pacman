//
// Created by Marie Van Nuffel on 14/12/2025.
//


#include "view/ConcreteEntityFactory.h"

// Pacman
std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createPacmanView(PacManModel* m) {
    auto view = std::make_shared<PacmanView>(m);
    std::shared_ptr<Observer> obs = std::static_pointer_cast<Observer>(view);
    return { view, obs };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createGhostView(GhostModel* m) {
    auto view = std::make_shared<GhostView>(m);
    return { view, nullptr };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createCoinView(CoinModel* m) {
    auto view = std::make_shared<CoinView>(m);
    std::shared_ptr<Observer> obs = std::static_pointer_cast<Observer>(view);
    return { view, obs };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createFruitView(FruitModel* m) {
    auto view = std::make_shared<FruitView>(m);
    std::shared_ptr<Observer> obs = std::static_pointer_cast<Observer>(view);
    return { view, obs };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createGhostDoorView(GhostDoorModel* m) {
    auto view = std::make_shared<GhostDoorView>(m);
    return { view, nullptr };
}