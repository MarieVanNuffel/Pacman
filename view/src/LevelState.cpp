//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/LevelState.h"

#include <iostream>

#include "view/PacManView.h"
#include "view/GhostView.h"
#include "view/CoinView.h"
#include "view/FruitView.h"
#include "view/GhostDoorView.h"
#include "view/MazeView.h"  // nieuwe MazeView met sprites


LevelState::LevelState(sf::RenderWindow& win, StateManager& sm)
    : State(win, sm), camera(0,0,0,0)
{
    factory = std::make_shared<ConcreteEntityFactory>();
    auto score = std::make_shared<Score>();
    world = std::make_shared<World>(factory, score);

    mazeView = std::make_unique<MazeView>(world.get());

    const auto& maze = world->getMaze();
    camera = Camera(
        window.getSize().x,
        window.getSize().y,
        maze[0].size(),
        maze.size()
    );
}



void LevelState::update(float dt) {
    world->update(dt);

    world->getPacmanView()->updateSprite(dt); // update de animatie van pacman
    for (auto& gv : world->getGhostViews()) // update de animatie van de ghosts
        gv->updateSprite(dt);

    for (auto& cv : world->getCoinViews())
        cv->updateSprite(dt);
}


void LevelState::render() {
    const auto& maze = world->getMaze();
    camera = Camera(window.getSize().x, window.getSize().y, maze[0].size(), maze.size());

    // reset SFML view to default so we draw in window pixels
    window.setView(window.getDefaultView());

    drawMaze();
    drawEntities();
}

void LevelState::drawMaze() {
    // Teken nu alleen de sprite-based maze
    mazeView->draw(window, camera);
}

void LevelState::drawEntities() {
    // PACMAN
    world->getPacmanView()->draw(window, camera);

    // GHOSTS
    for (auto& gv : world->getGhostViews()) {
        gv->draw(window, camera);
    }

    // COINS
    for (auto& cv : world->getCoinViews()) {
        cv->draw(window, camera);
    }

    // FRUITS
    for (auto& fv : world->getFruitViews()) {
        fv->draw(window, camera);
    }
}


void LevelState::handleInput(sf::Event& ev)
{
    if (ev.type == sf::Event::KeyPressed) {
        auto pacman = world->getPacman();

        if (ev.key.code == sf::Keyboard::Up) {
        pacman->setDirection(Direction::UP);
        pacman->setDesiredDirection(Direction::UP);
        }

        else if (ev.key.code == sf::Keyboard::Down) {
            pacman->setDirection(Direction::DOWN);
            pacman->setDesiredDirection(Direction::DOWN);
        }

        else if (ev.key.code == sf::Keyboard::Left) {
            pacman->setDirection(Direction::LEFT);
            pacman->setDesiredDirection(Direction::LEFT);
        }
        else if (ev.key.code == sf::Keyboard::Right) {
            pacman->setDirection(Direction::RIGHT);
            pacman->setDesiredDirection(Direction::RIGHT);
        }
    }
}