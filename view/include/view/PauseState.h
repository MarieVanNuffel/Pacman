#ifndef PACMANGAME_PAUSESTATE_H
#define PACMANGAME_PAUSESTATE_H
#pragma once

#include "view/State.h"
#include <SFML/Graphics.hpp>

class PauseState : public State {
public:
  PauseState(sf::RenderWindow &window, StateManager &sm,
             const sf::Texture &lastFrame);
  ~PauseState() override = default;

  void update(float dt) override;
  void render() override;
  void handleInput(sf::Event &ev) override;

private:
  // 'screenshot' van de game
  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;

  // UI / overlay
  sf::RectangleShape pauseOverlay;
  sf::RectangleShape blurBackground;
  sf::Font font;
  sf::Text pauseText;

  // scale factor
  float uiScale = 1.0f;

  // helper functie
  void drawPauseScreen();
};

#endif // PACMANGAME_PAUSESTATE_H