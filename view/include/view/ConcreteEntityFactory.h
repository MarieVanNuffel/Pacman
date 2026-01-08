//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CONCRETEENTITYFACTORY_H
#define PACMANGAME_CONCRETEENTITYFACTORY_H

#pragma once

#include "GhostDoorView.h"
#include "GhostView.h"
#include "PacmanView.h"
#include "logic/IEntityFactory.h"
#include <memory>

namespace logic {
class PacManModel;
class GhostModel;
class CoinModel;
class FruitModel;
class GhostDoorModel;
class Observer;
} // namespace logic

namespace view {

/**
 * @brief Concrete implementatie van IEntityFactory
 *
 * Maakt views en observers voor concrete entity types.
 */
class ConcreteEntityFactory : public logic::IEntityFactory {
public:
  /**
   * @brief Maakt pacman view + observer
   * @param m Pacman model
   * @return Pair van EntityView en Observer
   */
  std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
  createPacmanView(std::shared_ptr<logic::PacManModel> m) override;

  /**
   * @brief Maakt ghost view
   * @param m Ghost model
   * @return Pair van EntityView en nullptr observer, want krijgt geen events
   */
  std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
  createGhostView(std::shared_ptr<logic::GhostModel> m) override;

  /**
   * @brief Maakt coin view + observer
   * @param m Coin model
   * @return Pair van EntityView en Observer
   */
  std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
  createCoinView(std::shared_ptr<logic::CoinModel> m) override;

  /**
   * @brief Maakt fruit view + observer
   * @param m Fruit model
   * @return Pair van EntityView en Observer
   */
  std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
  createFruitView(std::shared_ptr<logic::FruitModel> m) override;

  /**
   * @brief Maakt ghostdoor view
   * @param m GhostDoor model
   * @return Pair van EntityView en nullptr observer, want krijgt geen events
   */
  std::pair<std::shared_ptr<EntityView>, std::shared_ptr<logic::Observer>>
  createGhostDoorView(std::shared_ptr<logic::GhostDoorModel> m) override;
};
} // namespace view

#endif // PACMANGAME_CONCRETEENTITYFACTORY_H