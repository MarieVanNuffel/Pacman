//
// Created by Marie Van Nuffel on 22/12/2025.
//

#ifndef PACMANGAME_GHOSTDOORVIEW_H
#define PACMANGAME_GHOSTDOORVIEW_H

#pragma once
#include "Camera.h"
#include "EntityView.h"
#include <SFML/Graphics.hpp>

namespace logic {
class GhostDoorModel;
}

namespace view {

/**
 * @brief View voor de ghostdoor
 *
 * Tekent de deur die de ghost box afsluit.
 */
class GhostDoorView : public EntityView {
public:
  /**
   * @brief Constructor
   * @param m GhostDoorModel
   */
  explicit GhostDoorView(const std::shared_ptr<logic::GhostDoorModel> &m);

  /**
   * @brief Tekent de ghostdoor
   */
  void draw(sf::RenderWindow &win, const Camera &cam) override;

  /**
   * @brief Update van sprite, maar beweegt niet dus doet niets
   */
  void updateSprite(double dt) override;

private:
  std::weak_ptr<logic::GhostDoorModel> model;
};
} // namespace view

#endif // PACMANGAME_GHOSTDOORVIEW_H