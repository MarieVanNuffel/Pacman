//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_MENUSTATE_H
#define PACMANGAME_MENUSTATE_H


#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class MenuState : public State {
public:
    MenuState(sf::RenderWindow& window, StateManager& sm);

    void handleInput(sf::Event& ev) override;
    void update(float dt) override;
    void render() override;

private:
    sf::Font font;
    sf::Text title;
    sf::Text playButton;
    sf::Text highScoreText;

    // Animatie sprites
    sf::Texture pacmanTexture;
    sf::Texture ghostTexture;
    sf::Sprite pacmanSprite;
    std::vector<sf::Sprite> ghostSprites;

    // Animatie variabelen
    float animationTimer;
    float animationSpeed;
    float pacmanPositionX;
    float ghostSpacing;
    bool isAnimating;
    int currentFrame;
    float ghostColorOffset;

    float uiScale = 1.f;
    static constexpr float BASE_WIDTH  = 800.f;
    static constexpr float BASE_HEIGHT = 600.f;

    sf::RectangleShape fadeRect;
    float fadeAlpha = 255.f;
    bool fadingIn = true;

};



#endif //PACMANGAME_MENUSTATE_H