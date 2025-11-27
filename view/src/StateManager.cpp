//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/StateManager.h"

namespace view {

    StateManager::StateManager(sf::RenderWindow& window)
        : window_(window) {}

    void StateManager::update() {
        if (!states_.empty())
            states_.back()->update();
    }

    void StateManager::draw() {
        if (!states_.empty())
            states_.back()->draw(window_);
    }

}
