//
// Created by Marie Van Nuffel on 14/12/2025.
//

#include "view/ConcreteEntityFactory.h"
#include "view/CoinView.h"
#include "view/FruitView.h"

namespace view {
    // Pacman
    std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<logic::Observer>>
    ConcreteEntityFactory::createPacmanView(std::shared_ptr<logic::PacManModel> m) {
        auto v = std::make_shared<PacmanView>(m);
        std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(v);
        return { v, obs };
    }

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    ConcreteEntityFactory::createGhostView(std::shared_ptr<logic::GhostModel> m) {
        auto v = std::make_shared<GhostView>(m);
        std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(v);
        return { v, obs };
    }

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    ConcreteEntityFactory::createCoinView(std::shared_ptr<logic::CoinModel> m) {
        auto view = std::make_shared<CoinView>(m);
        std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(view);
        return { view, obs };
    }

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    ConcreteEntityFactory::createFruitView(std::shared_ptr<logic::FruitModel> m) {
        auto view = std::make_shared<FruitView>(m);
        std::shared_ptr<logic::Observer> obs = std::static_pointer_cast<logic::Observer>(view);
        return { view, obs };
    }

    std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
    ConcreteEntityFactory::createGhostDoorView(std::shared_ptr<logic::GhostDoorModel> m) {
        auto view = std::make_shared<GhostDoorView>(m);
        return { view, nullptr };
    }
}