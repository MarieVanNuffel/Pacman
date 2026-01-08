//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_COINVIEW_H
#define PACMANGAME_COINVIEW_H

#pragma once
#include "Camera.h"
#include "EntityView.h"
#include "logic/Observer.h"

namespace logic {
class CoinModel;
}

namespace view {

/**
 * @brief View voor coin entity
 *
 * Tekent een coin en reageert op coin events via observer.
 */
class CoinView : public EntityView, public logic::Observer {
public:
  /**
   * @brief Constructor
   * @param m Coin model
   */
  explicit CoinView(const std::shared_ptr<logic::CoinModel> &m);

  /**
   * @brief Teken de coin
   * @param win RenderWindow
   * @param cam Camera voor world to pixel conversie
   */
  void draw(sf::RenderWindow &win, const Camera &cam) override;

  /**
   * @brief Update sprite per frame, maar statisch dus doet niets
   * @param dt Delta time
   */
  void updateSprite(double dt) override;

  /**
   * @brief Ontvangt events en past view aan
   * @param event Event id
   */
  void onNotify(int event) override;

private:
  std::weak_ptr<logic::CoinModel> model;
  bool visible = true;
};
} // namespace view

#endif // PACMANGAME_COINVIEW_H