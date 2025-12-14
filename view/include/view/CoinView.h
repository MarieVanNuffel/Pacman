//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_COINVIEW_H
#define PACMANGAME_COINVIEW_H

#pragma once
#include "EntityView.h"
#include "logic/CoinModel.h"
#include "Camera.h"
#include "logic/Observer.h"

class CoinView : public EntityView, public Observer {
public:
    CoinView(CoinModel* m);

    void draw(sf::RenderWindow& win, const Camera& cam) override;
    void updateSprite(double dt) override;

    void onNotify(int event) override;

private:
    CoinModel* model;
    bool visible;
};



#endif //PACMANGAME_COINVIEW_H