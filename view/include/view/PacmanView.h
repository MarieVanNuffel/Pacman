//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_PACMANVIEW_H
#define PACMANGAME_PACMANVIEW_H

#pragma once

#include "EntityView.h"
#include "Camera.h"

class PacManModel;

class PacmanView : public EntityView {
public:
    PacmanView(PacManModel* m);
    void draw(sf::RenderWindow& win, const Camera& cam) override;
    void updateSprite(double dt) override;
private:
    PacManModel* model;
    sf::Sprite sprite;
    sf::Texture texture;
    double animTime = 0.0;
};


#endif //PACMANGAME_PACMANVIEW_H