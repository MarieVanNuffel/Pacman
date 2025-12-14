//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/GhostView.h"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Texture.hpp>


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
    if (!model) return;

    double sizeW = 0.9; // 0.9 van een cel
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



