#include "view/Game.h"
#include "view/LevelState.h"
#include "view/MenuState.h"
#include "view/StateManager.h"
#include "logic/Stopwatch.h"

Game::Game()
    : window(sf::VideoMode(800, 800), "Pac-Man")
{
    stateManager.pushState(
        std::make_shared<MenuState>(window, stateManager)
    );
}

void Game::run()
{
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            stateManager.handleInput(ev);
        }

        double dt = Stopwatch::instance().tick();
        stateManager.update(dt);

        window.clear();
        stateManager.render();
        window.display();
    }
}
