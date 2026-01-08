//
// Created by Marie Van Nuffel on 30/11/2025.
//

#ifndef PACMANGAME_MAZEVIEW_H
#define PACMANGAME_MAZEVIEW_H

#pragma once

#include "Camera.h"
#include "logic/World.h"
#include <SFML/Graphics.hpp>

namespace view {

/**
 * @brief View van de maze
 *
 * MazeView tekent de maze dat in world is gegeven
 */
class MazeView {
public:
  /**
   * @brief Constructor
   * @param w Pointer naar de World die de maze bevat
   */
  explicit MazeView(const logic::World *w);

  /**
   * @brief Tekent de volledige maze volgens de vector dat in world is gegeven
   */
  void draw(sf::RenderWindow &win, const Camera &cam);

private:
  const logic::World *world;
};
} // namespace view

#endif // PACMANGAME_MAZEVIEW_H