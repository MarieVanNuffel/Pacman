//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/CoinView.h"
#include "../include/view/Camera.h"
#include <iostream>

CoinView::CoinView(CoinModel* m)
    : EntityView(m), model(m)
{
    if (!texture.loadFromFile("view/assets/Coin.png"))
    {
        std::cerr << "Kan Coin.png niet laden!" << std::endl;
    }

    sprite.setTexture(texture);

    // Sprite centreren
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void CoinView::updateSprite(double dt)
{
    animTime += dt;

    // Placeholder animatie (kan later uitgebreider)
    if (animTime > 0.25)
        animTime = 0.0;
}

void CoinView::draw(sf::RenderWindow& win, const Camera& cam)
{
    if (!model)
        return;

    double x = model->getX();
    double y = model->getY();

    // Zet wereldcoördinaten om naar pixels
    sf::FloatRect rect = cam.worldToPixels(x, y, 0.1, 0.1);

    sprite.setPosition(rect.left + rect.width / 2.f,
                       rect.top  + rect.height / 2.f);

    // Schaal sprite zodat hij in één tegel past
    sprite.setScale(rect.width / sprite.getTexture()->getSize().x,
                    rect.height / sprite.getTexture()->getSize().y);

    win.draw(sprite);
}
