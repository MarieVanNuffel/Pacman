//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_MENUSTATE_H
#define PACMANGAME_MENUSTATE_H
//* Voor het design heb ik het internet gebruikt om op te zoeken hoe de
//originele arcade game eruit zag.
// * Voor het design/de animatie heb ik AI gebruikt om SFML functies op te
// zoeken voor hoe ik het kon implementeren.

#pragma once
#include <memory>

#include "State.h"
#include <SFML/Graphics.hpp>

/**
 * @brief Hoofdmenu van pacman.
 *
 * Voor het design heb ik het internet gebruikt om op te zoeken hoe de originele
 * arcade game eruit zag. Voor het design/de animatie heb ik AI gebruikt om SFML
 * functies op te zoeken voor hoe ik het kon implementeren.
 *
 * Het design bevat de titel in de originele kleuren met daaronder een animatie
 * van de ghosts die achter pacman zweven. Er is ook een play button die van
 * kleur verandert als je met je muis erover gaat en de highscores met 1 in
 * goud, 2 in zilver en 3 in brons.
 *
 * @param window
 * @param sm
 */
class MenuState : public State {
public:
  /**
   * @brief Constructor
   */
  MenuState(sf::RenderWindow &window, StateManager &sm);

  /**
   * @brief Verwerkt input in het menu. ( op play klikken = start level)
   */
  void handleInput(sf::Event &ev) override;

  /**
   * @brief Update animaties en zorgt voor een fade.
   */
  void update(float dt) override;

  /**
   * @brief Tekent het menu met het hele design op het scherm.
   */
  void render() override;

private:
  sf::Font font;
  sf::Text title;
  sf::Text playButton;
  sf::Text highScoreText;

  // Animatie sprites
  std::shared_ptr<sf::Texture> pacmanTexturePtr;
  std::shared_ptr<sf::Texture> ghostTexturePtr;
  sf::Sprite pacmanSprite;
  std::vector<sf::Sprite> ghostSprites;

  // Animatie variabelen
  float animationTimer;
  float animationSpeed;
  float pacmanPositionX;
  float ghostSpacing;
  bool isAnimating;
  int currentFrame;
  float ghostColorOffset;

  float uiScale = 1.f;
  static constexpr float BASE_WIDTH = 800.f;
  static constexpr float BASE_HEIGHT = 600.f;

  sf::RectangleShape fadeRect;
  float fadeAlpha = 255.f;
  bool fadingIn = true;
};

#endif // PACMANGAME_MENUSTATE_H