//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_MENUSTATE_H
#define PACMANGAME_MENUSTATE_H


#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class MenuState : public State {
public:
    MenuState(sf::RenderWindow& window, StateManager& sm);

    void handleInput(sf::Event& ev) override;
    void update(float dt) override;
    void render() override;

private:
    sf::Font font;
    sf::Text title;
    sf::Text playButton;
};



#endif //PACMANGAME_MENUSTATE_H