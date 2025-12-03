#include "view/Game.h"
#include "view/LevelState.h"

Game::Game()
    : window(sf::VideoMode(800,800), "Pac-Man")
{
    stateManager.changeState(std::make_shared<LevelState>(window));
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        stateManager.update(dt);

        window.clear();
        stateManager.render();
        window.display();
    }
}

