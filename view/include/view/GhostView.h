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
    GhostView(GhostModel* m);
    void draw(sf::RenderWindow& win, const Camera& cam) override;
    void updateSprite(double dt) override;
private:
    GhostModel* model;
    sf::Sprite sprite;
    sf::Texture texture;
    double animTime = 0.0;
};


#endif //PACMANGAME_GHOSTVIEW_H