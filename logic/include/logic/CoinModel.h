//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_COIN_H
#define PACMANGAME_COIN_H

#pragma once

#include "Entity.h"
#include "Subject.h"

namespace logic {

/**
 * @brief Model voor een coin
 */
class CoinModel : public Entity, public Subject {
public:
  CoinModel() = default;
  ~CoinModel() override = default;

  /**
   * @brief Coin is opgepakt, notify observers
   */
  void collect();

  /**
   * @brief Update functie
   *
   * Coin is statisch, dus doet niets
   * @param dt Delta time
   */
  void update(double dt) override;

  /**
   * @brief Reset coin, notify obervers
   */
  void reset();

  bool collected = false;
};

} // namespace logic

#endif // PACMANGAME_COIN_H