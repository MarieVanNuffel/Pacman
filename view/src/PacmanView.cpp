//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/PacmanView.h"
#include "logic/PacManModel.h"        // model class
#include <SFML/Graphics.hpp>

PacmanView::PacmanView(PacManModel* m)
    : EntityView(m), model(m)
{
    // Texture laden
    if (!texture.loadFromFile("view/assets/pacman.png")) {
        throw std::runtime_error("Failed to load pacman texture!");
    }

    sprite.setTexture(texture);
    sprite.setOrigin(
        texture.getSize().x / 2.0f,
        texture.getSize().y / 2.0f
    );
}

void PacmanView::draw(sf::RenderWindow& win, const Camera& cam)
{
    // Wereld → pixels conversie
    auto r = cam.worldToPixels(
        model->getX(),
        model->getY(),
        model->getW(),
        model->getH()
    );

    sprite.setPosition(r.left + r.width / 2.0f,
                       r.top  + r.height / 2.0f);

    win.draw(sprite);
}

void PacmanView::updateSprite(double dt)
{
    animTime += dt;

    // Kleine animatie: Pac-Man draait naar bewegingsrichting
    switch (model->getDir()) {
        case Direction::UP:    sprite.setRotation(270); break;
        case Direction::DOWN:  sprite.setRotation(90);  break;
        case Direction::LEFT:  sprite.setRotation(180); break;
        case Direction::RIGHT: sprite.setRotation(0);   break;
        default: break;
    }

    // Hier kun je eventueel sprite-sheet animatie toevoegen
}

