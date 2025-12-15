//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_PACMANVIEW_H
#define PACMANGAME_PACMANVIEW_H

#pragma once

#include "EntityView.h"
#include "Camera.h"
#include "logic/Direction.h"

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

    // pacman grootte sprite sheet
    static constexpr int FRAME_SIZE = 15;
    static constexpr int FRAMES_PER_DIR = 2;

    // animatie
    double animTimer = 0.0;
    double frameTime = 0.15;
    bool mouthOpen = true;
};


#endif //PACMANGAME_PACMANVIEW_H