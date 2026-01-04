//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_COINVIEW_H
#define PACMANGAME_COINVIEW_H

#pragma once
#include "EntityView.h"
#include "Camera.h"
#include "logic/Observer.h"

namespace logic {
    class CoinModel;
}

namespace view {
    class CoinView : public EntityView, public logic::Observer {
    public:
        explicit CoinView(std::shared_ptr<logic::CoinModel> m);

        void draw(sf::RenderWindow& win, const Camera& cam) override;
        void updateSprite(double dt) override;

        void onNotify(int event) override;

    private:
        logic::CoinModel* model;
        bool visible = true;
    };
}


#endif //PACMANGAME_COINVIEW_H