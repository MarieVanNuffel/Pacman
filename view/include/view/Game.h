//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H


#pragma once
#include <SFML/Graphics.hpp>
#include "logic/World.h"
#include "view/StateManager.h"

namespace view {

    class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window_;
        StateManager stateManager_;
    };

}



#endif //PACMAN_GAME_H