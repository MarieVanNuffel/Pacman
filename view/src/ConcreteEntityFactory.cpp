//
// Created by Marie Van Nuffel on 14/12/2025.
//


#include "view/ConcreteEntityFactory.h"

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createPacmanView(PacManModel* m) {
    auto concrete = std::make_shared<PacmanView>(m);
    // PacmanView does not act as Observer in this design, so second = nullptr
    return { std::static_pointer_cast<EntityView>(concrete), std::static_pointer_cast<Observer>(concrete) };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createGhostView(GhostModel* m) {
    auto concrete = std::make_shared<GhostView>(m);
    // GhostView is a visual-only view here (no Observer) — return nullptr as second.
    // If your GhostView actually inherits Observer, change nullptr -> static_pointer_cast<Observer>(concrete)
    return { std::static_pointer_cast<EntityView>(concrete), nullptr };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createCoinView(CoinModel* m) {
    auto concrete = std::make_shared<CoinView>(m);
    // CoinView inherits Observer, so provide both pointers
    return { std::static_pointer_cast<EntityView>(concrete), std::static_pointer_cast<Observer>(concrete) };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createFruitView(FruitModel* m) {
    auto concrete = std::make_shared<FruitView>(m);
    // FruitView inherits Observer, so provide both pointers
    return { std::static_pointer_cast<EntityView>(concrete), std::static_pointer_cast<Observer>(concrete) };
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<Observer>>
ConcreteEntityFactory::createGhostDoorView(GhostDoorModel* m) {
    auto concrete = std::make_shared<GhostDoorView>(m);
    return { std::static_pointer_cast<EntityView>(concrete), nullptr };
}