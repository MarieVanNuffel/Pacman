//
// Created by Marie Van Nuffel on 27/11/2025.
//
#include "../include/view/CoinView.h"

#include <cmath>

#include "../include/view/Camera.h"
#include "logic/World.h"

CoinView::CoinView(CoinModel* m)
    : EntityView(m), model(m), visible(true)
{}

void CoinView::draw(sf::RenderWindow& win, const Camera& cam)
{
    if (!model || !visible) return;

    double sizeW = 0.5; // halve cel
    double sizeH = 0.5;

    // verschuif middelpunt naar linker-bovenhoek van de coin
    sf::FloatRect rect = cam.worldToPixels(model->getX() - sizeW/2.0,
                                           model->getY() - sizeH/2.0,
                                           sizeW, sizeH);

    float radius = std::min(rect.width, rect.height) / 3.f;

    sf::CircleShape coin(radius);
    coin.setFillColor(sf::Color::Yellow);

    // centreer coin in rect
    coin.setOrigin(radius, radius);  // zet origin in het midden van de cirkel
    coin.setPosition(rect.left + rect.width / 2.f,
                     rect.top  + rect.height / 2.f);

    win.draw(coin);
}


void CoinView::updateSprite(double dt) {
}


void CoinView::onNotify(int event)
{
    if (event == 1) { // 1 = coin collected event
        visible = false;
    }
}