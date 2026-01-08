#include "view/Game.h"
#include "logic/Stopwatch.h"
#include "view/LevelState.h"
#include "view/MenuState.h"
#include "view/StateManager.h"

Game::Game() : window(sf::VideoMode(800, 800), "PacMan"), stateManager(window) {
  stateManager.pushState(
      std::make_shared<MenuState>(window, stateManager) // menustate pushen
  );
  isFullscreen = false;
}

void Game::run() {
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) { // event handling
      if (ev.type == sf::Event::Closed)
        window.close();

      // handle fullscreen toggle (F11)
      if (ev.type == sf::Event::KeyPressed &&
          ev.key.code == sf::Keyboard::F11) {
        toggleFullscreen();
      }

      // als window resized wordt, reset de view
      if (ev.type == sf::Event::Resized) {
        window.setView(window.getDefaultView());
      }

      stateManager.handleInput(ev);
    }

    double dt = logic::Stopwatch::instance().tick(); // tijd sinds vorige frame
    stateManager.update(dt);

    window.clear();
    stateManager.render(); // statemanager tekent de huidige state
    window.display();
  }
}

void Game::toggleFullscreen() {
  // wissel van/naar fullscreen
  isFullscreen = !isFullscreen;

  if (isFullscreen) {
    // fullscreen met desktop resolutie
    sf::VideoMode dm = sf::VideoMode::getDesktopMode();
    window.create(dm, "PacMan", sf::Style::Fullscreen);
  } else {
    // terug naar windowed mode
    window.create(sf::VideoMode(800, 800), "PacMan",
                  sf::Style::Close | sf::Style::Titlebar);
  }

  // reset SFML view zodat er geen verkeerde zoom of schaling is
  window.setView(window.getDefaultView());
}