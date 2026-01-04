//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#pragma once

#include <SFML/Graphics.hpp>
#include "view/StateManager.h"
#include "view/LevelState.h"

class Game {
public:
    Game();
    void run();

    void toggleFullscreen();
private:
    sf::RenderWindow window;
    StateManager stateManager;

    bool isFullscreen;
};



#endif //PACMAN_GAME_H