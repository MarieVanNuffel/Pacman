//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/LevelState.h"
#include "view/PacManView.h"
#include "view/GhostView.h"
#include "view/CoinView.h"
#include "view/FruitView.h"
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

void LevelState::handleInput(sf::Event& ev)
{
    if (ev.type == sf::Event::KeyPressed) {
        auto pacman = world->getPacman();

        if (ev.key.code == sf::Keyboard::Up)
            pacman->setDirection(Direction::UP);
        else if (ev.key.code == sf::Keyboard::Down)
            pacman->setDirection(Direction::DOWN);
        else if (ev.key.code == sf::Keyboard::Left)
            pacman->setDirection(Direction::LEFT);
        else if (ev.key.code == sf::Keyboard::Right)
            pacman->setDirection(Direction::RIGHT);
    }
}
