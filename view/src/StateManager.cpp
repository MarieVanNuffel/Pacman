//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/StateManager.h"
#include "view/State.h"

StateManager::StateManager(sf::RenderWindow &win) : window(win) {
  score = std::make_shared<logic::Score>(); // 1 score voor de hele game
}

void StateManager::pushState(std::shared_ptr<State> state) {
  states.push_back(std::move(state));
}

void StateManager::popState() {
  if (!states.empty())
    states.pop_back();
}

void StateManager::changeState(std::shared_ptr<State> state) {
  states.clear();
  pushState(std::move(state));
}

void StateManager::handleInput(sf::Event &ev) {
  if (!states.empty())
    states.back()->handleInput(ev);
}

void StateManager::update(double dt) {
  if (!states.empty())
    states.back()->update(dt);
}

void StateManager::render() {
  if (!states.empty())
    states.back()->render();
}
