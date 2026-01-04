//
// Created by Marie Van Nuffel on 22/12/2025.
//

#ifndef PACMANGAME_GHOSTDOORVIEW_H
#define PACMANGAME_GHOSTDOORVIEW_H

#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "EntityView.h"

namespace logic {
    class GhostDoorModel;
}

namespace view {
    class GhostDoorView : public EntityView {
    public:
        explicit GhostDoorView(std::shared_ptr<logic::GhostDoorModel> m);

        void draw(sf::RenderWindow& win, const Camera& cam) override;
        void updateSprite(double dt) override;

    private:
        logic::GhostDoorModel* model;
    };
}

#endif //PACMANGAME_GHOSTDOORVIEW_H