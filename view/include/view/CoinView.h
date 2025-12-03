//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_COINVIEW_H
#define PACMANGAME_COINVIEW_H

#include "EntityView.h"
#include "logic/CoinModel.h"


class CoinView : public EntityView {
public:
    CoinView(CoinModel* m);
    void draw(sf::RenderWindow& win, const Camera& cam) override;
    void updateSprite(double dt) override;
private:
    CoinModel* model;
    sf::Sprite sprite;
    sf::Texture texture;
    double animTime = 0.0;
};


#endif //PACMANGAME_COINVIEW_H