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
    LevelState(sf::RenderWindow& window);

    void update(float dt) override;
    void render() override;

private:
    sf::RenderWindow& window;
    std::shared_ptr<World> world;
    std::shared_ptr<ConcreteEntityFactory> factory;
    Camera camera;

    // MazeView voor sprite-based muren
    std::unique_ptr<MazeView> mazeView;

    float tileW;
    float tileH;

    void drawMaze();
    void drawEntities();
};

#endif //PACMAN_LEVELSTATE_H
