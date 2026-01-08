//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_ENTITY_H
#define PACMANGAME_ENTITY_H

#pragma once

#include "Direction.h"
#include "Rect.h"
#include <memory>

namespace logic {

/**
 * @brief Basis class voor alle entities
 *
 * Bevat positie, size, direction en speed.
 * Wordt gebruikt door alle entities.
 */
class Entity : public std::enable_shared_from_this<Entity> {
protected:
  double x = 0.0, y = 0.0; // wereld positie
  double w = 0.1, h = 0.1; // size
  Direction direction = Direction::NONE;
  Direction desiredDirection = Direction::NONE;
  double speed = 0; // units per second

public:
  virtual ~Entity() = default;

  /**
   * @brief Update entity per tick
   * @param dt Delta time
   */
  virtual void update(double dt) = 0;

  // Getters

  /**
   * @brief Geeft x-positie terug
   * @return x-coördinaat
   */
  [[nodiscard]] double getX() const { return x; }

  /**
   * @brief Geeft y-positie terug
   * @return y-coördinaat
   */
  [[nodiscard]] double getY() const { return y; }

  /**
   * @brief Geeft breedte terug
   * @return Width
   */
  [[nodiscard]] double getW() const { return w; }

  /**
   * @brief Geeft hoogte terug
   * @return Height
   */
  [[nodiscard]] double getH() const { return h; }

  /**
   * @brief Geeft huidige direction terug
   * @return Direction
   */
  [[nodiscard]] Direction getDirection() const { return direction; }

  /**
   * @brief Geeft desired direction terug
   * @return Desired direction
   */
  [[nodiscard]] Direction getDesiredDirection() const {
    return desiredDirection;
  }

  /**
   * @brief Zet huidige direction
   * @param d Nieuwe direction
   */
  void setDirection(Direction d) { direction = d; }

  /**
   * @brief Geeft bounding box terug
   * @return Rect bounds
   */
  [[nodiscard]] Rect getBounds() const { return Rect{x, y, w, h}; }

  /**
   * @brief Geeft movement speed terug
   * @return Speed
   */
  [[nodiscard]] double getSpeed() const { return speed; };

  /**
   * @brief Zet entity positie
   * @param nx Nieuwe x
   * @param ny Nieuwe y
   */
  void setPosition(double nx, double ny) {
    x = nx;
    y = ny;
  }
};
} // namespace logic

#endif // PACMANGAME_ENTITY_H