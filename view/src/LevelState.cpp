//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/LevelState.h"

#include <iomanip>
#include <iostream>

#include "view/PacManView.h"
#include "view/GhostView.h"
#include "view/CoinView.h"
#include "view/FruitView.h"
#include "view/GhostDoorView.h"
#include "view/MazeView.h"  // nieuwe MazeView met sprites
#include "view/MenuState.h"
#include "view/StateManager.h"


LevelState::LevelState(sf::RenderWindow& win, StateManager& sm)
    : State(win, sm), camera(0,0,0,0)
{
    factory = std::make_shared<ConcreteEntityFactory>();
    world = std::make_shared<World>(factory, stateManager.getScore());

    mazeView = std::make_unique<MazeView>(world.get());

    const auto& maze = world->getMaze();
    camera = Camera(
        window.getSize().x,
        window.getSize().y,
        maze[0].size(),
        maze.size()
    );

    // ✅ Load font
    if (!font.loadFromFile("view/assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
    }

    // ✅ Setup score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10 * uiScale, 30 * uiScale);

    // ✅ Setup lives text
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);

    // ✅ Load Pac-Man sprite for lives display
    if (!lifeTexture.loadFromFile("view/assets/pacman.png")) {
        std::cerr << "Could not load pacman.png for lives!" << std::endl;
    }
    lifeSprite.setTexture(lifeTexture);
    lifeSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    lifeSprite.setScale(2.5f * uiScale, 2.5f * uiScale);   // 2x is klassiek arcade

}



void LevelState::update(float dt) {
    world->update(dt);

    world->getPacmanView()->updateSprite(dt); // update de animatie van pacman
    for (auto& gv : world->getGhostViews()) // update de animatie van de ghosts
        gv->updateSprite(dt);

    for (auto& cv : world->getCoinViews())
        cv->updateSprite(dt);

    // ✅ Update UI
    updateUI();

    // ✅ Check game over
    if (world->getPacman()->isGameOver()) {
        stateManager.getScore()->saveHighScores();
        stateManager.changeState(
            std::make_shared<MenuState>(window, stateManager)
        );
    }

}

void LevelState::updateUI() {
    auto score = world->getScore();

    // ✅ Format score with leading zeros
    std::ostringstream oss;
    oss << "SCORE:" << std::setw(6) << std::setfill('0') << score->getCurrentScore();
    scoreText.setString(oss.str());

    // ✅ Update lives text (optional, we'll draw sprites)
    livesText.setString("LIVES:");
}


void LevelState::render() {
    float scaleX = window.getSize().x / BASE_WIDTH;
    float scaleY = window.getSize().y / BASE_HEIGHT;
    uiScale = std::min(scaleX, scaleY);

    const auto& maze = world->getMaze();
    camera = Camera(window.getSize().x, window.getSize().y, maze[0].size(), maze.size());

    // reset SFML view to default so we draw in window pixels
    window.setView(window.getDefaultView());

    drawMaze();
    drawEntities();
    drawUI();
}

void LevelState::drawUI() {
    // UI werkt in scherm-coördinaten
    window.setView(window.getDefaultView());

    // --- SCORE ---
    scoreText.setCharacterSize(static_cast<unsigned>(24 * uiScale));
    scoreText.setPosition(20.f, 40.f);
    window.draw(scoreText);

    // --- LIVES ---
    livesText.setCharacterSize(static_cast<unsigned>(24 * uiScale));

    // Vaste positie: 20 pixels van links, 100 pixels van onder
    float livesY = window.getSize().y - 45.f * uiScale;
    livesText.setPosition(20.f, livesY);
    window.draw(livesText);

    // --- LIVES ICONS ---
    lifeSprite.setScale(2.5f * uiScale, 2.5f * uiScale);
    int lives = world->getPacman()->getLives();

    float iconSize = 40.f * uiScale;
    float baseX = 20.f + livesText.getGlobalBounds().width + 20.f * uiScale;

    // Centreer de icons verticaal met de tekst
    float textHeight = livesText.getLocalBounds().height;
    float iconHeight = lifeSprite.getGlobalBounds().height;
    float verticalOffset = (textHeight - iconHeight) / 2.0f;

    for (int i = 0; i < lives; i++) {
        lifeSprite.setPosition(
            baseX + i * (iconSize + 5.f * uiScale),
            livesY + verticalOffset
        );
        window.draw(lifeSprite);
    }
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

    for (auto& gdv: world->getGhostDoorViews()) {
        gdv->draw(window, camera);
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