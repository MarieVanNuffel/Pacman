//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#pragma once

#include <SFML/Graphics.hpp>
#include "view/StateManager.h"
#include "view/Levelstate.h"
#include "Camera.h"

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow window;
    StateManager stateManager;
};



#endif //PACMAN_GAME_H