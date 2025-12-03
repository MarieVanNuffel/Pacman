//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_GHOSTMODEL_H
#define PACMANGAME_GHOSTMODEL_H

#pragma once


#include "Entity.h"
#include "Direction.h"
#include <chrono>


class World; // forward


class GhostModel : public Entity {
public:
    enum class Mode { Waiting, Chase, Fear, Eaten };
private:
    Mode mode = Mode::Waiting;
    Direction locked = Direction::LEFT;
    double speed = 0.8;
    double startX = 0, startY = 0;

    const World* worldRef = nullptr;

    Direction decideDirection();

public:
    GhostModel();
    void update(double dt) override;

    void setWorld(const World* w) { worldRef = w; };
    void setMode(Mode m);
    Mode getMode() const;


    void setStartPosition(double sx, double sy) { x = sx; y = sy; startX = sx; startY = sy; }

    Direction computeLockedDir(const World& world);

};



#endif //PACMANGAME_GHOSTMODEL_H