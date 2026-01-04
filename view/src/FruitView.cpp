//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/FruitView.h"
#include "../include/view/Camera.h"
#include <iostream>

#include "logic/Events.h"

FruitView::FruitView(logic::FruitModel* m)
    : EntityView(m), model(m)
{
    if (!texture.loadFromFile("view/assets/pacman.png"))
    {
        std::cerr << "Kan fruit.png niet laden!" << std::endl;
    }

    sprite.setTexture(texture);

    int spriteX = 0;
    int spriteY = 128;

    sprite.setTextureRect(sf::IntRect(spriteX, spriteY, FRAME_SIZE, FRAME_SIZE));

    // Sprite centreren
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void FruitView::updateSprite(double dt)
{
}

void FruitView::draw(sf::RenderWindow& win, const Camera& cam)
{
    if (!model) return;
    if (!visible) return; // belangrijke check: als fruit verzameld is, niet tekenen

    double sizeW = 0.5; // halve cel
    double sizeH = 0.5;

    sf::FloatRect rect = cam.worldToPixels(model->getX() - sizeW/2.0,
                                       model->getY() - sizeH/2.0,
                                       sizeW, sizeH);
    sprite.setPosition(rect.left + rect.width/2.f,
                       rect.top  + rect.height/2.f);

    // uniform scaling om vervorming te voorkomen
    float sx = rect.width / sprite.getLocalBounds().width;
    float sy = rect.height / sprite.getLocalBounds().height;
    float s = std::min(sx, sy);
    sprite.setScale(s * 1.4, s * 1.4);

    win.draw(sprite);
}

void FruitView::onNotify(int event)
{
    if (event == static_cast<int>(logic::Event::FRUIT_EATEN)) {
        visible = false;
    } else {
        visible = true;
    }
}
