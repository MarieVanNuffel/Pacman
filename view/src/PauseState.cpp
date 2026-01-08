#include "view/PauseState.h"
#include "view/StateManager.h"
#include <iostream>

PauseState::PauseState(sf::RenderWindow &window, StateManager &sm,
                       const sf::Texture &snapshot)
    : State(window, sm) {
  uiScale = 1.0f;

  // de 'screenshot' van de game
  backgroundTexture = snapshot;
  backgroundSprite.setTexture(backgroundTexture);
  backgroundSprite.setPosition(0.f, 0.f);

  pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));
  blurBackground.setFillColor(sf::Color(255, 255, 255, 30));

  if (!font.loadFromFile("view/assets/fonts/PressStart2P-Regular.ttf")) {
    throw std::runtime_error(
        "Failed to load font: view/assets/fonts/PressStart2P-Regular.ttf");
  }

  pauseText.setFont(font);
  pauseText.setString("PAUSED");
  pauseText.setCharacterSize(48);
  pauseText.setFillColor(sf::Color::Yellow);
  pauseText.setStyle(sf::Text::Bold);
}

void PauseState::update(float /*dt*/) {
  // niet gebruikt
}

void PauseState::handleInput(sf::Event &ev) {
  if (ev.type == sf::Event::KeyPressed) {
    if (ev.key.code == sf::Keyboard::Escape) {
      // door escape terug naar het spel
      stateManager.popState();
    }
  } else if (ev.type == sf::Event::Resized) {
    pauseOverlay.setSize(sf::Vector2f(static_cast<float>(window.getSize().x),
                                      static_cast<float>(window.getSize().y)));
    const sf::Vector2u size = window.getSize();
    if (size.x > 0 && size.y > 0) {
      backgroundTexture.create(size.x, size.y);
      backgroundTexture.update(window);
      backgroundSprite.setTexture(backgroundTexture, true);
    }
  }
}

void PauseState::render() {
  const float BASE_WIDTH = 800.0f;
  const float BASE_HEIGHT = 600.0f;
  float scaleX = window.getSize().x / BASE_WIDTH;
  float scaleY = window.getSize().y / BASE_HEIGHT;
  uiScale = std::min(scaleX, scaleY);

  drawPauseScreen();
}

void PauseState::drawPauseScreen() {
  // Sla de huidige view op
  sf::View originalView = window.getView();

  // Schakel over naar default view voor UI
  window.setView(window.getDefaultView());

  // teken de screenshot
  window.draw(backgroundSprite);

  // Update overlay grootte naar huidige window grootte
  pauseOverlay.setSize(sf::Vector2f(window.getSize()));

  // Teken semi-transparante overlay (blur)
  window.draw(pauseOverlay);

  blurBackground.setSize(sf::Vector2f(window.getSize()));
  window.draw(blurBackground);

  // Centreer de pause tekst
  sf::FloatRect textBounds = pauseText.getLocalBounds();
  pauseText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                      textBounds.top + textBounds.height / 2.0f);
  pauseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

  // Maak de tekst groter voor pause scherm
  pauseText.setCharacterSize(static_cast<unsigned>(64 * uiScale));
  pauseText.setOutlineColor(sf::Color::Black);
  pauseText.setOutlineThickness(4.0f * uiScale);

  window.draw(pauseText);

  // Tekst voor terug te hervatten naar het spel
  sf::Text instructionText;
  instructionText.setFont(font);
  instructionText.setString("Press ESC to resume");
  instructionText.setCharacterSize(static_cast<unsigned>(20 * uiScale));
  instructionText.setFillColor(sf::Color::White);

  sf::FloatRect instrBounds = instructionText.getLocalBounds();
  instructionText.setOrigin(instrBounds.left + instrBounds.width / 2.0f,
                            instrBounds.top + instrBounds.height / 2.0f);
  instructionText.setPosition(window.getSize().x / 2.0f,
                              window.getSize().y / 2.0f + 80.f * uiScale);

  window.draw(instructionText);

  // Herstel de originele view
  window.setView(originalView);
}
