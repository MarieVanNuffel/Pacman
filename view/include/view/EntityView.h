//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_ENTITYVIEW_H
#define PACMANGAME_ENTITYVIEW_H


#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Camera.h"

class Entity;

class EntityView {
public:
    EntityView(Entity* model);
    virtual void draw(sf::RenderWindow&, const Camera&) = 0;
    virtual void updateSprite(double dt) = 0;
protected:
    Entity* model;
};



#endif //PACMANGAME_ENTITYVIEW_H