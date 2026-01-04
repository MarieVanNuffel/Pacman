//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_ENTITY_H
#define PACMANGAME_ENTITY_H


#pragma once


#include "Rect.h"
#include "Direction.h"
#include <memory>

namespace logic {
    class Entity {
    protected:
        double x = 0.0, y = 0.0; // normalized coordinates [-1,1]
        double w = 0.1, h = 0.1; // size in normalized units
        Direction direction = Direction::NONE;
        Direction desiredDirection = Direction::NONE;
        double speed = 0; // normalized units per second

    public:
        virtual ~Entity() = default;


        virtual void update(double dt) = 0;


        double getX() const { return x; }
        double getY() const { return y; }
        double getW() const { return w; }
        double getH() const { return h; }
        Direction getDirection() const { return direction; }
        Direction getDesiredDirection() const { return desiredDirection; }
        void setDirection(Direction d) { direction = d; }
        Rect getBounds() const { return Rect{x, y, w, h}; }
        double getSpeed() const { return speed; };


        void setPosition(double nx, double ny) { x = nx; y = ny; }

    };
}

#endif //PACMANGAME_ENTITY_H