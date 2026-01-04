//
// Created by Marie Van Nuffel on 14/12/2025.
//

#include "view/MenuState.h"
#include <cmath>
#include <iomanip>
#include "view/LevelState.h"
#include <iostream>
#include <sstream>
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
    title.setFillColor({255,215,0});
    title.setPosition(200, 100);
    title.setOutlineColor({sf::Color::Black});
    title.setOutlineThickness(4.f);

    // PLAY BUTTON
    playButton.setFont(font);
    playButton.setString("PLAY");
    playButton.setCharacterSize(36);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(350, 300);

    // HIGHSCORES
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor({207, 23, 23});
    highScoreText.setPosition(450, 500);
    highScoreText.setOutlineColor({89, 4, 4});
    highScoreText.setOutlineThickness(1.5);


    // FADE
    fadeRect.setSize(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 255));

    // ANIMATIE INITIALISATIE
    if (!pacmanTexture.loadFromFile("view/assets/pacman.png")) {
        std::cerr << "Failed to load pacman texture for menu animation!\n";
    }

    // Gebruik dezelfde texture voor ghosts
    ghostTexture = pacmanTexture;

    // Pac-Man sprite instellen
    pacmanSprite.setTexture(pacmanTexture);
    pacmanSprite.setTextureRect(sf::IntRect(0, 0, 15, 15)); // Eerste frame
    pacmanSprite.setOrigin(8, 8);
    pacmanSprite.setScale(2.5f, 2.5f); // Groter voor menu

    // Maak 4 ghost sprites
    for (int i = 0; i < 4; i++) {
        sf::Sprite ghost;
        ghost.setTexture(ghostTexture);

        // Stel de juiste kleur in (zelfde als in GhostView)
        int rectTop = 0;
        switch (i) {
            case 0: rectTop = 64; break;  // rood (Blinky)
            case 1: rectTop = 80; break;  // roze (Pinky)
            case 2: rectTop = 96; break;  // lichtblauw (Inky)
            case 3: rectTop = 112; break; // oranje (Clyde)
        }

        ghost.setTextureRect(sf::IntRect(0, rectTop, 16, 16));
        ghost.setOrigin(8, 8);
        ghost.setScale(2.5f, 2.5f);
        ghostSprites.push_back(ghost);
    }

    // Animatie variabelen
    animationTimer = 0.0f;
    animationSpeed = 100.0f; // Pixels per seconde
    pacmanPositionX = -100.0f; // Start off-screen links
    ghostSpacing = 50.0f; // Afstand tussen ghosts
    isAnimating = true;
    currentFrame = 0;
    ghostColorOffset = 0.0f;

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
    if (fadingIn) {
        fadeAlpha -= 150.f * dt;
        float alphaFactor = 1.0f - fadeAlpha / 255.f;
        title.setFillColor(sf::Color(255, 255, 0, static_cast<sf::Uint8>(255 * alphaFactor)));
        playButton.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alphaFactor)));
        if (fadeAlpha <= 0.f) {
            fadeAlpha = 0.f;
            fadingIn = false;
        }
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    }

    // ANIMATIE UPDATE
    if (isAnimating) {
        // Update timer voor frame animatie
        static float frameTimer = 0.0f;
        frameTimer += dt;

        // Pac-Man mond animatie (elke 0.15 seconden)
        if (frameTimer >= 0.15f) {
            frameTimer = 0.0f;
            currentFrame = (currentFrame + 1) % 2;

            // Update Pac-Man frame
            int frameX = currentFrame * 16;
            pacmanSprite.setTextureRect(sf::IntRect(frameX, 0, 15, 15));

            // Update ghost frames (dicht/open mond)
            for (auto& ghost : ghostSprites) {
                auto rect = ghost.getTextureRect();
                int frameX = currentFrame * 16;
                ghost.setTextureRect(sf::IntRect(frameX, rect.top, 16, 16));
            }
        }

        // Beweging van rechts naar links
        pacmanPositionX += animationSpeed * dt;

        // Bereken positie van de laatste ghost
        int lastGhostIndex = ghostSprites.size() - 1;
        float lastGhostX = pacmanPositionX - (lastGhostIndex + 1) * ghostSpacing * uiScale - 30.f;

        // Als de LAATSTE ghost helemaal voorbij is, reset naar rechts
        float windowWidth = window.getSize().x;
        if (lastGhostX > windowWidth + 200.0f) {
            // Reset alle posities naar links (off-screen)
            pacmanPositionX = -200.0f;
        }

        // Ghosts volgen Pac-Man met een delay
        ghostColorOffset += dt * 10.0f;
    }
}

void MenuState::render()
{
    float scaleX = window.getSize().x / BASE_WIDTH;
    float scaleY = window.getSize().y / BASE_HEIGHT;
    uiScale = std::min(scaleX, scaleY);

    // Zorg dat we tekenen in standaard window-pixelruimte (geen oude SFML view)
    window.setView(window.getDefaultView());

    // ACHTERGROND
    sf::Vector2u ws = window.getSize();
    sf::RectangleShape bg(sf::Vector2f(ws.x, ws.y));
    bg.setFillColor(sf::Color(10, 10, 50));

    float centerX = static_cast<float>(ws.x) * 0.5f;

    // TITEL - Eerst tekst instellen, dan achtergrond berekenen
    title.setCharacterSize(static_cast<unsigned>(64 * uiScale));
    auto titleBounds = title.getLocalBounds();
    float titleX = centerX - (titleBounds.left + titleBounds.width) / 2.f;
    float titleY = static_cast<float>(ws.y) * 0.15f;
    title.setPosition(titleX, titleY);

    // TITEL ACHTERGROND - Nu met twee lagen
    float bgPaddingX = 40.f * uiScale;
    float bgPaddingY = 20.f * uiScale;
    float bgWidth = titleBounds.width + 2 * bgPaddingX;
    float bgHeight = titleBounds.height + 2 * bgPaddingY;

    // 1. DONKER ORANJE OUTLINE (externe rand)
    sf::RectangleShape titleOutline;
    titleOutline.setSize(sf::Vector2f(bgWidth, bgHeight));
    titleOutline.setOrigin(bgWidth / 2.f, bgHeight / 2.f);
    float titleCenterX = titleX + titleBounds.left + titleBounds.width / 2.f;
    float titleCenterY = titleY + titleBounds.top + titleBounds.height / 2.f;
    titleOutline.setPosition(titleCenterX, titleCenterY);
    titleOutline.setFillColor(sf::Color(226, 67, 47));  // Donker oranje
    titleOutline.setOutlineColor(sf::Color::Black);     // Zwarte outline
    titleOutline.setOutlineThickness(3.f * uiScale);    // Dunne zwarte outline

    // 2. ORANJE ACHTERGROND (interne vulkleur)
    float innerPadding = 9.f * uiScale;  // Hoe dik de "outline" moet zijn
    sf::RectangleShape titleBackGround;
    titleBackGround.setSize(sf::Vector2f(
        bgWidth - 2 * innerPadding,
        bgHeight - 2 * innerPadding
    ));
    titleBackGround.setOrigin(
        titleBackGround.getSize().x / 2.f,
        titleBackGround.getSize().y / 2.f
    );
    titleBackGround.setPosition(titleCenterX, titleCenterY);
    titleBackGround.setFillColor(sf::Color(237, 138, 58));  // Oranje
    titleBackGround.setOutlineColor(sf::Color::Black);      // Optioneel: zwarte outline voor binnenste
    titleBackGround.setOutlineThickness(3.f * uiScale);     // Optioneel


    // PLAY BUTTON
    playButton.setCharacterSize(static_cast<unsigned>(36 * uiScale));
    auto btnBounds = playButton.getLocalBounds();
    float btnX = centerX - (btnBounds.left + btnBounds.width) / 2.f;
    float btnY = static_cast<float>(ws.y) * 0.45f;
    playButton.setPosition(btnX, btnY);

    auto mouse = sf::Mouse::getPosition(window);
    if (playButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
        playButton.setFillColor(sf::Color::Yellow);
    } else {
        playButton.setFillColor(sf::Color::White);
    }

    // HIGHSCORES
    auto scores = stateManager.getScore()->getHighScores();

    std::ostringstream oss;
    oss << "HIGHSCORES";
    highScoreText.setString(oss.str());
    highScoreText.setCharacterSize(static_cast<unsigned>(24 * uiScale));

    // Bereken positie voor de titel "HIGHSCORES"
    auto hscBounds = highScoreText.getLocalBounds();
    float hscX = centerX - (hscBounds.left + hscBounds.width) / 2.f;
    float hscY = static_cast<float>(ws.y) * 0.65f;
    highScoreText.setPosition(hscX, hscY);

    // Maak en positioneer de score teksten
    std::vector<sf::Text> scoreTexts;
    for (int i = 0; i < scores.size(); i++) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(static_cast<unsigned>(20 * uiScale)); // iets kleiner voor de cijfers

        // Formatteer score met leading zeros
        std::ostringstream scoreOss;
        scoreOss << std::setw(2) << std::setfill('0') << (i + 1) << ". "
                 << std::setw(6) << std::setfill('0') << scores[i];
        t.setString(scoreOss.str());

        // Bereken bounds en centreer
        auto tBounds = t.getLocalBounds();
        float tX = centerX - (tBounds.left + tBounds.width) / 2.f;
        float tY = hscY + hscBounds.height + 20.f * uiScale + i * 35.f * uiScale;
        t.setPosition(tX, tY);

        switch(i) {
            case 0: t.setFillColor(sf::Color(255, 215, 0)); break; // goud
            case 1: t.setFillColor(sf::Color(192, 192, 192)); break; // zilver
            case 2: t.setFillColor(sf::Color(205, 127, 50)); break; // brons
            default: t.setFillColor(sf::Color::White); break;
        }
        scoreTexts.push_back(t);
    }

    // Teken alles in de juiste volgorde
    window.draw(bg);

    // Bereken Y-positie voor animatie (onder titel)
    float titleBottomY = titleY + titleBounds.height + 20.f * uiScale;
    float animationY = titleBottomY + 50.f * uiScale;

    // Teken animatie
    if (isAnimating) {
        // Teken Pac-Man
        pacmanSprite.setPosition(pacmanPositionX, animationY);
        pacmanSprite.setScale(2.5f * uiScale, 2.5f * uiScale);
        window.draw(pacmanSprite);

        // Teken ghosts met offset (achter Pac-Man)
        for (int i = 0; i < ghostSprites.size(); i++) {
            // Bereken positie met sinus voor natuurlijke beweging
            float offsetX = pacmanPositionX - (i + 1) * ghostSpacing * uiScale;
            float offsetY = animationY + sin(ghostColorOffset + i) * 10.f * uiScale;

            ghostSprites[i].setPosition(offsetX - 30.f, offsetY);
            ghostSprites[i].setScale(2.5f * uiScale, 2.5f * uiScale);
            window.draw(ghostSprites[i]);
        }
    }

    window.draw(titleOutline);      // Eerst donker oranje met zwarte rand
    window.draw(titleBackGround);   // Dan oranje achtergrond
    window.draw(title);            // Dan de tekst
    window.draw(playButton);
    window.draw(highScoreText);
    for (auto& t : scoreTexts) {
        window.draw(t);
    }

    if (fadeAlpha > 0.f) {
        window.draw(fadeRect);
    }
}
