//
// Created by Marie Van Nuffel on 27/11/2025.
//
#include "../include/view/CoinView.h"

#include <cmath>

#include "../include/view/Camera.h"
#include "logic/Events.h"
#include "logic/World.h"

namespace view {
    CoinView::CoinView(std::shared_ptr<logic::CoinModel> m)
        : EntityView(m), model(m)
    {}

    void CoinView::draw(sf::RenderWindow& win, const Camera& cam)
    {
        auto m = model.lock();
        if (!m || !visible) return; // kijken of de coin nog visible is

        double sizeW = 0.5; // halve cel
        double sizeH = 0.5;

        // tile center
        double tileCenterX = m->getX();
        double tileCenterY = m->getY();

        // genormalizeerd
        auto [nx, ny] = cam.tileToNormalized(tileCenterX, tileCenterY);

        double wFrac = sizeW / static_cast<double>(cam.mazeWidth);
        double hFrac = sizeH / static_cast<double>(cam.mazeHeight);

        sf::FloatRect rect = cam.normalizedToPixels(nx, ny, wFrac, hFrac);

        float radius = std::min(rect.width, rect.height) / 3.f; // radius van coin

        sf::CircleShape coin(radius); // coin tekenen
        coin.setFillColor(sf::Color::Yellow);

        // centreer coin
        coin.setOrigin(radius, radius);  // zet origin in het midden van de cirkel
        coin.setPosition(rect.left + rect.width / 2.f,
                         rect.top  + rect.height / 2.f);

        win.draw(coin);
    }


    void CoinView::updateSprite(double dt) {
        // statisch, dus doet niets
    }


    void CoinView::onNotify(int event)
    {
        if (event == static_cast<int>(logic::Event::COIN_COLLECTED)) {
            visible = false;
        } else if (event == static_cast<int>(logic::Event::COIN_RESPAWN)) {
            visible = true;
        }
    }
}