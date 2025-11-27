//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STATEMANAGER_H
#define PACMAN_STATEMANAGER_H

#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "view/State.h"

namespace view {

    class StateManager {
    public:
        StateManager(sf::RenderWindow& window);

        void update();
        void draw();

        template<typename T, typename... Args>
        void push(Args&&... args) {
            states_.push_back(std::make_unique<T>(*this, window_, std::forward<Args>(args)...));
        }

    private:
        sf::RenderWindow& window_;
        std::vector<std::unique_ptr<State>> states_;
    };

}

#endif //PACMAN_STATEMANAGER_H