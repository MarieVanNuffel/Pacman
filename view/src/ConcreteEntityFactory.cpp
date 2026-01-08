//
// Created by Marie Van Nuffel on 14/12/2025.
//

#include "view/ConcreteEntityFactory.h"
#include "view/CoinView.h"
#include "view/FruitView.h"
#include "view/GhostDoorView.h"

namespace view {
// Pacman
std::pair<std::shared_ptr<view::EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createPacmanView(std::shared_ptr<logic::PacManModel> m) {
  auto v = std::make_shared<PacmanView>(m); // pacmanview wordt aangemaakt
  std::shared_ptr<logic::Observer> obs =
      std::static_pointer_cast<logic::Observer>(
          v); // view implementeert observer
  return {v, obs};
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createGhostView(std::shared_ptr<logic::GhostModel> m) {
  auto v = std::make_shared<GhostView>(m); // ghostview wordt aangemaakt
  return {v, nullptr}; // ghosts maken geen gebruik van een observer, dus
                       // nullpointer wordt teruggegeven
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createCoinView(std::shared_ptr<logic::CoinModel> m) {
  auto view = std::make_shared<CoinView>(m); // coinview wordt aangemaakt
  std::shared_ptr<logic::Observer> obs =
      std::static_pointer_cast<logic::Observer>(
          view); // view implementeert observer
  return {view, obs};
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createFruitView(std::shared_ptr<logic::FruitModel> m) {
  auto view = std::make_shared<FruitView>(m); // fruitview wordt aangemaakt
  std::shared_ptr<logic::Observer> obs =
      std::static_pointer_cast<logic::Observer>(
          view); // view implementeert observer
  return {view, obs};
}

std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
ConcreteEntityFactory::createGhostDoorView(
    std::shared_ptr<logic::GhostDoorModel> m) {
  auto view =
      std::make_shared<GhostDoorView>(m); // ghostdoorview wordt aangemaakt
  return {view, nullptr}; // ghosts maken geen gebruik van een observer, dus
                          // nullpointer wordt teruggegeven
}
} // namespace view