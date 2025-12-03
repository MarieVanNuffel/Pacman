//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/StateManager.h"
#include "view/State.h"

void StateManager::changeState(std::shared_ptr<State> newState) {
    current = newState;
}

void StateManager::update(float dt) {
    if (current) current->update(dt);
}

void StateManager::render() {
    if (current) current->render();
}

