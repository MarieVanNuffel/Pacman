//
// Created by Marie Van Nuffel on 14/12/2025.
//

#include "view/MenuState.h"
#include "view/LevelState.h"
#include <iostream>

#include "view/StateManager.h"

MenuState::MenuState(sf::RenderWindow& win, StateManager& sm)
    : State(win, sm)
{
    // FONT
    if (!font.loadFromFile("view/assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "Failed to load font!\n";
    }

    // TITEL
    title.setFont(font);
    title.setString("PAC-MAN");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(200, 100);

    // PLAY BUTTON
    playButton.setFont(font);
    playButton.setString("PLAY");
    playButton.setCharacterSize(36);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(350, 300);

}

void MenuState::handleInput(sf::Event& ev)
{
    if (ev.type == sf::Event::MouseButtonPressed) {
        auto mouse = sf::Mouse::getPosition(window);
        if (playButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
            stateManager.changeState(
                std::make_shared<LevelState>(window, stateManager)
            );
        }
    }

    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
        stateManager.changeState(
            std::make_shared<LevelState>(window, stateManager)
        );
    }
}

void MenuState::update(float dt)
{
    // eventueel animaties voor sprites of knoppen
}

void MenuState::render()
{
    // Zorg dat we tekenen in standaard window-pixelruimte (geen oude SFML view)
    window.setView(window.getDefaultView());

    // positioneer tekst en knop relatief aan windowgrootte (centraal)
    sf::Vector2u ws = window.getSize();
    float centerX = static_cast<float>(ws.x) * 0.5f;

    // TITEL: centreer horizontaal; positie verticaal op 15% van hoogte
    auto titleBounds = title.getLocalBounds();
    float titleX = centerX - (titleBounds.left + titleBounds.width) / 2.f;
    float titleY = static_cast<float>(ws.y) * 0.15f;
    title.setPosition(titleX, titleY);

    // PLAY BUTTON: centreer horizontaal; positie verticaal op 50% van hoogte
    auto btnBounds = playButton.getLocalBounds();
    float btnX = centerX - (btnBounds.left + btnBounds.width) / 2.f;
    float btnY = static_cast<float>(ws.y) * 0.50f;
    playButton.setPosition(btnX, btnY);

    // Teken
    window.draw(title);
    window.draw(playButton);
}
