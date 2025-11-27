//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STATE_H
#define PACMAN_STATE_H


#pragma once
#include <SFML/Graphics.hpp>

namespace view {

    class State {
    public:
        virtual ~State() = default;
        virtual void update() = 0;
        virtual void draw(sf::RenderWindow& win) = 0;
    };

}



#endif //PACMAN_STATE_H