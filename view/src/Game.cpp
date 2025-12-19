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
    isFullscreen = false;
}

void Game::run()
{
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            // handle fullscreen toggle (F11)
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::F11) {
                toggleFullscreen();
            }

            // als window resized wordt, reset de view (veiligheid)
            if (ev.type == sf::Event::Resized) {
                window.setView(window.getDefaultView());
            }

            stateManager.handleInput(ev);
        }

        double dt = Stopwatch::instance().tick();
        stateManager.update(dt);

        window.clear();
        stateManager.render();
        window.display();
    }
}

void Game::toggleFullscreen()
{
    // wissel state
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        // echte fullscreen met desktop resolutie
        sf::VideoMode dm = sf::VideoMode::getDesktopMode();
        window.create(dm, "Pac-Man", sf::Style::Fullscreen);
    } else {
        // terug naar windowed mode (800x800 zoals aanvankelijk)
        window.create(sf::VideoMode(800, 800), "Pac-Man", sf::Style::Close | sf::Style::Titlebar);
    }

    // reset SFML view zodat oude zoom/viewport geen effect meer heeft
    window.setView(window.getDefaultView());
}