//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/Game.h"
#include <SFML/Window.hpp>

namespace view {

Game::Game()
    : window_(sf::VideoMode(1000, 1000), "Pac-Man"),
      stateManager_(window_) {}

void Game::run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_.close();
        }

        stateManager_.update();
        window_.clear();
        stateManager_.draw();
        window_.display();
    }
}

}
