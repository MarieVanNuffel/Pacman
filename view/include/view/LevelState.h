//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_LEVELSTATE_H
#define PACMAN_LEVELSTATE_H

#pragma once

#include "ConcreteEntityFactory.h"
#include "State.h"
#include "logic/World.h"
#include "view/MazeView.h"
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @brief Game state voor een level
 *
 * LevelState beheert de spel-logica, rendering en input tijdens het spelen.
 * Het verbindt world met de verschillende views en UI-elementen.
 */
class LevelState : public State {
public:
  /**
   * @brief Constructor
   * @param window Referentie naar het SFML window
   * @param sm StateManager voor state-overgangen
   */
  LevelState(sf::RenderWindow &window, StateManager &sm);

  /**
   * @brief Verwerkt input events (keyboard)
   */
  void handleInput(sf::Event &ev) override;

  /**
   * @brief Update game logica en animaties
   */
  void update(float dt) override;

  /**
   * @brief Render maze, entities en UI
   */
  void render() override;

private:
  std::shared_ptr<logic::World> world;
  std::shared_ptr<view::ConcreteEntityFactory> factory;
  view::Camera camera;

  std::unique_ptr<view::MazeView> mazeView;
  std::shared_ptr<view::EntityView> pacmanView;
  std::vector<std::shared_ptr<view::EntityView>> ghostViews;
  std::vector<std::shared_ptr<view::EntityView>> coinViews;
  std::vector<std::shared_ptr<view::EntityView>> fruitViews;
  std::vector<std::shared_ptr<view::EntityView>> ghostDoorViews;

  float tileW{};
  float tileH{};

  // UI elements
  sf::Font font;
  sf::Text scoreText;
  sf::Text livesText;
  sf::Sprite lifeSprite;
  std::shared_ptr<sf::Texture> lifeTexturePtr;

  // voor pauseState
  sf::Texture lastFrameTexture;

  const float BASE_WIDTH = 800.f;
  const float BASE_HEIGHT = 600.f;
  float uiScale = 1.f;

  // Helper functies

  /**
   * @brief Tekent de maze
   */
  void drawMaze();

  /**
   * @brief tekent entities
   */
  void drawEntities();

  /**
   * @brief Tekent de UI
   */
  void drawUI();

  /**
   * @brief Update de UI
   */
  void updateUI();
};

#endif // PACMAN_LEVELSTATE_H
