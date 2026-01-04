//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_LEVELSTATE_H
#define PACMAN_LEVELSTATE_H

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "State.h"
#include "logic/World.h"
#include "ConcreteEntityFactory.h"
#include "view/MazeView.h"

class LevelState : public State {
public:
    LevelState(sf::RenderWindow& window, StateManager& sm);

    void handleInput(sf::Event& ev) override;
    void update(float dt) override;
    void render() override;

private:
    std::shared_ptr<logic::World> world;
    std::shared_ptr<ConcreteEntityFactory> factory;
    Camera camera;

    std::unique_ptr<MazeView> mazeView;
    std::shared_ptr<EntityView> pacmanView;
    std::vector<std::shared_ptr<EntityView>> ghostViews;
    std::vector<std::shared_ptr<EntityView>> coinViews;
    std::vector<std::shared_ptr<EntityView>> fruitViews;
    std::vector<std::shared_ptr<EntityView>> ghostDoorViews;

    float tileW{};
    float tileH{};

    // ✅ UI elements
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Sprite lifeSprite;
    sf::Texture lifeTexture;

    bool isPaused;
    sf::Text pauseText;
    sf::RectangleShape pauseOverlay;
    sf::RectangleShape blurBackground;
    void drawPauseScreen();

    const float BASE_WIDTH  = 800.f;
    const float BASE_HEIGHT = 600.f;
    float uiScale = 1.f;


    void drawMaze();
    void drawEntities();
    void drawUI(); // ✅ Nieuwe functie
    void updateUI(); // ✅ Update score/lives display
};

#endif //PACMAN_LEVELSTATE_H
