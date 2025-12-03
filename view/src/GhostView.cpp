//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/GhostView.h"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>


//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/GhostView.h"
#include <iostream>

GhostView::GhostView(GhostModel* m)
    : EntityView(m), model(m)
{
    if (!texture.loadFromFile("view/assets/ghost.png"))
    {
        std::cerr << "Kan ghost.png niet laden!" << std::endl;
    }

    sprite.setTexture(texture);

    // Optioneel (centreren voor betere uitlijning)
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void GhostView::updateSprite(double dt)
{
    animTime += dt;

    // Placeholder animatie (later kan je frames toevoegen)
    if (animTime > 0.2)
        animTime = 0.0;
}

void GhostView::draw(sf::RenderWindow& win, const Camera& cam)
{
    if (!model)
        return;

    double x = model->getX();
    double y = model->getY();

    // PacMan gebruikt wereldcoördinaten [-1, 1], dus camera moet ze omzetten
    sf::FloatRect rect = cam.worldToPixels(x, y, 0.1, 0.1);

    sprite.setPosition(rect.left + rect.width / 2.f,
                       rect.top  + rect.height / 2.f);

    // Schaal sprite naar tegelgrootte
    sprite.setScale(rect.width / sprite.getTexture()->getSize().x,
                    rect.height / sprite.getTexture()->getSize().y);

    win.draw(sprite);
}

