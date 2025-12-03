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


class State;

class StateManager {
public:
    void changeState(std::shared_ptr<State> newState);
    void update(float dt);
    void render();

private:
    std::shared_ptr<State> current;
};


#endif //PACMAN_STATEMANAGER_H