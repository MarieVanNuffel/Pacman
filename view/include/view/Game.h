//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#pragma once

#include "view/LevelState.h"
#include "view/StateManager.h"
#include <SFML/Graphics.hpp>

/**
 * @brief Hoofdklasse van het spel
 *
 * Beheert SFML, de game loop en de state manager.
 */
class Game {
public:
  /**
   * @brief Constructor
   *
   * Initialiseert het venster en zet de start state naar menu state.
   */
  Game();

  /**
   * @brief Start de game loop
   *
   * Verwerkt input, update logica en rendert de actieve state.
   */
  void run();

  /**
   * @brief Wisselt tussen fullscreen en klein window
   */
  void toggleFullscreen();

private:
  sf::RenderWindow window;
  StateManager stateManager;

  bool isFullscreen;
};

#endif // PACMAN_GAME_H