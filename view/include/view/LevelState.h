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
#include "view/MazeView.h"  // nieuwe MazeView

class LevelState : public State {
public:
    LevelState(sf::RenderWindow& window, StateManager& sm);

    void handleInput(sf::Event& ev) override;
    void update(float dt) override;
    void render() override;

private:
    std::shared_ptr<World> world;
    std::shared_ptr<ConcreteEntityFactory> factory;
    Camera camera;
    std::unique_ptr<MazeView> mazeView;

    float tileW;
    float tileH;

    // ✅ UI elements
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Sprite lifeSprite;
    sf::Texture lifeTexture;

    const float BASE_WIDTH  = 800.f;
    const float BASE_HEIGHT = 600.f;
    float uiScale = 1.f;


    void drawMaze();
    void drawEntities();
    void drawUI(); // ✅ Nieuwe functie
    void updateUI(); // ✅ Update score/lives display
};

#endif //PACMAN_LEVELSTATE_H
