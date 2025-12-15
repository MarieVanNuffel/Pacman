//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/PacmanView.h"
#include "logic/PacManModel.h"        // model class
#include <SFML/Graphics.hpp>

PacmanView::PacmanView(PacManModel* m)
    : EntityView(m), model(m)
{
    if (!texture.loadFromFile("view/assets/pacman.png")) {
        throw std::runtime_error("Failed to load pacman texture!");
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, FRAME_SIZE, FRAME_SIZE));
    sprite.setOrigin(FRAME_SIZE / 2.f, FRAME_SIZE / 2.f);
}


void PacmanView::draw(sf::RenderWindow& win, const Camera& cam)
{
    double sizeW = 0.9;
    double sizeH = 0.9;

    sf::FloatRect rect = cam.worldToPixels(model->getX() - sizeW/2.0,
                                       model->getY() - sizeH/2.0,
                                       sizeW, sizeH);
    sprite.setPosition(rect.left + rect.width/2.f,
                       rect.top  + rect.height/2.f);
    sprite.setScale(rect.width / sprite.getLocalBounds().width,
                    rect.height / sprite.getLocalBounds().height);


    win.draw(sprite);
}



void PacmanView::updateSprite(double dt)
{
    animTimer += dt;

    if (animTimer >= frameTime) {
        animTimer = 0.0;
        mouthOpen = !mouthOpen;
    }

    int frameX = mouthOpen ? 0 : FRAME_SIZE;
    int frameY = 0;

    switch (model->getDirection()) {
        case Direction::RIGHT: frameY = 0 * FRAME_SIZE; break;
        case Direction::LEFT:  frameY = 1 * FRAME_SIZE; break;
        case Direction::UP:    frameY = 2 * FRAME_SIZE; break;
        case Direction::DOWN:  frameY = 3 * FRAME_SIZE; break;
        default: return; // geen richting → animatie niet veranderen
    }

    sprite.setTextureRect(sf::IntRect(frameX, frameY, FRAME_SIZE, FRAME_SIZE));
}



