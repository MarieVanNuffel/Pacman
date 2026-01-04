//
// Created by Marie Van Nuffel on 22/12/2025.
//

#ifndef PACMANGAME_GHOSTDOORVIEW_H
#define PACMANGAME_GHOSTDOORVIEW_H

#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "EntityView.h"
#include "logic/GhostDoorModel.h"

class GhostDoorView : public EntityView {
public:
    GhostDoorView(logic::GhostDoorModel* model);

    void draw(sf::RenderWindow& win, const Camera& cam);
    void updateSprite(double dt) override;

private:
    logic::GhostDoorModel* model;
};


#endif //PACMANGAME_GHOSTDOORVIEW_H