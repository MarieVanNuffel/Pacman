//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STATEMANAGER_H
#define PACMAN_STATEMANAGER_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class State;

class StateManager {
public:
    void pushState(std::shared_ptr<State> state);
    void popState();
    void changeState(std::shared_ptr<State> state);

    void handleInput(sf::Event& ev);
    void update(double dt);
    void render();

private:
    std::vector<std::shared_ptr<State>> states;
};

#endif
