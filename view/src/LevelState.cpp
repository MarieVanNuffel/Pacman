//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/LevelState.h"

namespace view {

LevelState::LevelState(StateManager&, sf::RenderWindow&) {}

void LevelState::update() {
    world_.update();
}

void LevelState::draw(sf::RenderWindow& win) {
    // voorlopig niks (geen view)
    // straks roepen we entity views aan
}

}
