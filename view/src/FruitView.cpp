//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/FruitView.h"
#include "../include/view/Camera.h"
#include <iostream>

FruitView::FruitView(FruitModel* m)
    : EntityView(m), model(m), visible(true)
{
    if (!texture.loadFromFile("view/assets/fruit.png"))
    {
        std::cerr << "Kan fruit.png niet laden!" << std::endl;
    }

    sprite.setTexture(texture);

    // Sprite centreren
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void FruitView::updateSprite(double dt)
{
    animTime += dt;

    // Placeholder animatie (kan later uitgebreider)
    if (animTime > 0.25)
        animTime = 0.0;
}

void FruitView::draw(sf::RenderWindow& win, const Camera& cam)
{
    if (!model) return;

    double sizeW = 0.5; // halve cel
    double sizeH = 0.5;

    sf::FloatRect rect = cam.worldToPixels(model->getX() - sizeW/2.0,
                                       model->getY() - sizeH/2.0,
                                       sizeW, sizeH);
    sprite.setPosition(rect.left + rect.width/2.f,
                       rect.top  + rect.height/2.f);
    sprite.setScale(rect.width / sprite.getLocalBounds().width,
                    rect.height / sprite.getLocalBounds().height);


    win.draw(sprite);
}

void FruitView::onNotify(int event)
{
    if (event == 2) { // 2 = fruit collected event
        visible = false;
    }
}
