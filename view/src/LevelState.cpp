//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/LevelState.h"
#include "view/PacManView.h"
#include "view/GhostView.h"
#include "view/CoinView.h"
#include "view/FruitView.h"
#include "view/MazeView.h"  // nieuwe MazeView met sprites

LevelState::LevelState(sf::RenderWindow& win)
    : window(win), camera(window.getSize().x, window.getSize().y)
{
    factory = std::make_shared<ConcreteEntityFactory>();
    auto score = std::make_shared<Score>();

    world = std::make_shared<World>(factory, score);

    // Maak een MazeView aan voor sprite-based muren
    mazeView = std::make_unique<MazeView>(world.get());

    const auto& maze = world->getMaze();
    tileW = window.getSize().x / float(maze[0].size());
    tileH = window.getSize().y / float(maze.size());
}

void LevelState::update(float dt) {
    world->update(dt);
}

void LevelState::render() {
    drawMaze();
    drawEntities();
}

void LevelState::drawMaze() {
    // Teken nu alleen de sprite-based maze
    mazeView->draw(window, camera);
}

void LevelState::drawEntities() {
    // PACMAN
    auto pmView = factory->createPacmanView(world->getPacman().get());
    pmView->draw(window, camera);

    // GHOSTS
    for (auto& g : world->getGhosts()) {
        auto gv = factory->createGhostView(g.get());
        gv->draw(window, camera);
    }

    // COINS
    for (auto& c : world->getCoins()) {
        if (!c->collected) {
            auto cv = factory->createCoinView(c.get());
            cv->draw(window, camera);
        }
    }

    // FRUITS
    for (auto& f : world->getFruits()) {
        if (!f->collected) {
            auto fv = factory->createFruitView(f.get());
            fv->draw(window, camera);
        }
    }
}
