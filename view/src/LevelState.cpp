//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "view/LevelState.h"
#include "view/GhostView.h"
#include "view/MazeView.h"
#include "view/MenuState.h"
#include "view/StateManager.h"
#include <iomanip>
#include <iostream>

#include "view/Resource.h"


LevelState::LevelState(sf::RenderWindow& win, StateManager& sm)
    : State(win, sm), camera(0,0,0,0)
{
    factory = std::make_shared<view::ConcreteEntityFactory>();
    world = std::make_shared<logic::World>(factory, stateManager.getScore());
    world->spawnEntitiesForLevel();

    mazeView = std::make_unique<view::MazeView>(world.get());

    const auto& maze = world->getMaze();
    camera = view::Camera(
        window.getSize().x,
        window.getSize().y,
        maze[0].size(),
        maze.size()
    );

    // Font inladen
    if (!font.loadFromFile("view/assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
    }

    // Score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10 * uiScale, 30 * uiScale);

    // Lives text
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);

    // Pacman sprite voor bij de levens te zetten
    lifeTexturePtr = view::Resource::loadTexture("view/assets/pacman.png");
    if (lifeTexturePtr) lifeSprite.setTexture(*lifeTexturePtr);

    lifeSprite.setTexture(*lifeTexturePtr);
    lifeSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    lifeSprite.setScale(2.5f * uiScale, 2.5f * uiScale);

    // Pause systeem
    isPaused = false;

    // Wazige overlay (blur)
    pauseOverlay.setSize(sf::Vector2f(window.getSize()));
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparant zwart

    // Pauze tekst
    pauseText.setFont(font); // Zelfde font
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setStyle(sf::Text::Bold);

    // Maak een "blur" effect door een witte semi-transparante rechthoek
    blurBackground.setFillColor(sf::Color(255, 255, 255, 30));

    // Pacman view
    {
        auto pv = factory->createPacmanView(world->getPacman());
        pacmanView = pv.first;
        if (pv.second) world->getPacman()->addObserver(pv.second);
    }

    // Ghost views
    ghostViews.clear();
    for (auto& gm : world->getGhosts()) {
        auto gv = factory->createGhostView(gm);
        ghostViews.push_back(gv.first);
        if (gv.second) gm->addObserver(gv.second);
    }

    // Coin views
    coinViews.clear();
    for (auto& cm : world->getCoins()) {
        auto cv = factory->createCoinView(cm);
        coinViews.push_back(cv.first);
        if (cv.second) cm->addObserver(cv.second);
    }

    // Fruit views
    fruitViews.clear();
    for (auto& fm : world->getFruits()) {
        auto fv = factory->createFruitView(fm);
        fruitViews.push_back(fv.first);
        if (fv.second) fm->addObserver(fv.second);
    }

    // Ghostdoor views
    ghostDoorViews.clear();
    for (auto& gd : world->getGhostDoors()) {
        auto dv = factory->createGhostDoorView(gd);
        ghostDoorViews.push_back(dv.first);
    }
}



void LevelState::update(float dt) {
    // Alleen updaten als niet gepauzeerd
    if (!isPaused) {
        world->update(dt);

        pacmanView->updateSprite(dt);
        for (auto& gv : ghostViews)
            gv->updateSprite(dt);

        for (auto& cv : coinViews)
            cv->updateSprite(dt);

        updateUI();

        if (world->getPacman()->isGameOver()) {
            stateManager.getScore()->saveHighScores();
            stateManager.changeState(
                std::make_shared<MenuState>(window, stateManager)
            );
        }
    }
}

void LevelState::updateUI() {
    auto score = world->getScore();

    // Score met nullen vanvoor
    std::ostringstream oss;
    oss << "SCORE:" << std::setw(6) << std::setfill('0') << score->getCurrentScore();
    scoreText.setString(oss.str());

    // Lives text
    livesText.setString("LIVES:");
}


void LevelState::render() {
    float scaleX = window.getSize().x / BASE_WIDTH;
    float scaleY = window.getSize().y / BASE_HEIGHT;
    uiScale = std::min(scaleX, scaleY);

    const auto& maze = world->getMaze();
    camera = view::Camera(window.getSize().x, window.getSize().y, maze[0].size(), maze.size());

    window.setView(window.getDefaultView());

    drawMaze();
    drawEntities();
    drawUI();

    // Teken pauze overlay als gepauzeerd
    if (isPaused) {
        drawPauseScreen();
    }
}

void LevelState::drawUI() {
    // UI werkt in schermcoördinaten
    window.setView(window.getDefaultView());

    // Score
    scoreText.setCharacterSize(static_cast<unsigned>(24 * uiScale));
    scoreText.setPosition(20.f, 40.f);
    window.draw(scoreText);

    // Lives
    livesText.setCharacterSize(static_cast<unsigned>(24 * uiScale));

    // Positie
    float livesY = window.getSize().y - 45.f * uiScale;
    livesText.setPosition(20.f, livesY);
    window.draw(livesText);

    // Lives icoontjes
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
    mazeView->draw(window, camera);
}

void LevelState::drawEntities() {
    // pacman
    pacmanView->draw(window, camera);

    // ghosts
    for (auto& gv : ghostViews) {
        gv->draw(window, camera);
    }

    // coins
    for (auto& cv : coinViews) {
        cv->draw(window, camera);
    }

    // fruits
    for (auto& fv : fruitViews) {
        fv->draw(window, camera);
    }

    // ghostdoor
    for (auto& gdv: ghostDoorViews) {
        gdv->draw(window, camera);
    }
}


void LevelState::handleInput(sf::Event& ev)
{
    if (ev.type == sf::Event::KeyPressed) {
        // Escape voor pause
        if (ev.key.code == sf::Keyboard::Escape) {
            isPaused = !isPaused;
        }

        // Als niet gepauzeerd, verwerk de pacman input (richting)
        if (!isPaused) {
            auto pacman = world->getPacman();

            if (ev.key.code == sf::Keyboard::Up) {
                pacman->setDirection(logic::Direction::UP);
                pacman->setDesiredDirection(logic::Direction::UP);
            }
            else if (ev.key.code == sf::Keyboard::Down) {
                pacman->setDirection(logic::Direction::DOWN);
                pacman->setDesiredDirection(logic::Direction::DOWN);
            }
            else if (ev.key.code == sf::Keyboard::Left) {
                pacman->setDirection(logic::Direction::LEFT);
                pacman->setDesiredDirection(logic::Direction::LEFT);
            }
            else if (ev.key.code == sf::Keyboard::Right) {
                pacman->setDirection(logic::Direction::RIGHT);
                pacman->setDesiredDirection(logic::Direction::RIGHT);
            }
        }
    }
}

void LevelState::drawPauseScreen()
{
    // Sla de huidige view op
    sf::View originalView = window.getView();

    // Schakel over naar default view voor UI
    window.setView(window.getDefaultView());

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
    pauseText.setPosition(window.getSize().x / 2.0f,
                         window.getSize().y / 2.0f);

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