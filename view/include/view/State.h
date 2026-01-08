//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_STATE_H
#define PACMANGAME_STATE_H

#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

class State {
public:
  virtual ~State() = default;

  virtual void handleInput(sf::Event &ev) = 0;
  virtual void update(float dt) = 0;
  virtual void render() = 0;

protected:
  State(sf::RenderWindow &win, StateManager &sm)
      : window(win), stateManager(sm) {}

  sf::RenderWindow &window;
  StateManager &stateManager;
};

#endif // PACMANGAME_STATE_H