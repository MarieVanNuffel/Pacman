//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_PACMANMODEL_H
#define PACMANGAME_PACMANMODEL_H
#include "Entity.h"


#pragma once


#include "Entity.h"
#include "Direction.h"


class PacManModel : public Entity {
private:
    Direction dir = Direction::NONE; // None, Up, Down, Left, Right
    double speed = 1.0; // normalized units per second
    int lives = 3;

public:
    PacManModel();
    void setDirection(Direction d);
    Direction getDirection() const;
    void update(double dt) override;


    void reset();
    int getLives() const { return lives; }
    void loseLife();


};



#endif //PACMANGAME_PACMANMODEL_H