//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_GHOSTVIEW_H
#define PACMANGAME_GHOSTVIEW_H
#include <SFML/Graphics/RenderWindow.hpp>

#include "Camera.h"
#include "EntityView.h"
#include "logic/GhostModel.h"


class GhostView : public EntityView {
public:
    GhostView(logic::GhostModel* m);
    void draw(sf::RenderWindow& win, const Camera& cam) override;
    void updateSprite(double dt) override;
private:
    // animatie
    double animTimer = 0.0;
    double frameTime = 0.15;

    // sprite sheet frames
    static constexpr int FRAME_SIZE = 16;
    static constexpr int NUM_FRAMES = 2;
    int frameIndex = 0;
    int frames[NUM_FRAMES] = {0, 16}; // kolommen voor bewegingen

    // richting van Ghosts
    logic::GhostModel* model;
    sf::Sprite sprite;
    sf::Texture texture;

    logic::Direction lastDirection = logic::Direction::RIGHT;
};


#endif //PACMANGAME_GHOSTVIEW_H