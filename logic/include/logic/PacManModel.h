//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_PACMANMODEL_H
#define PACMANGAME_PACMANMODEL_H
#include "Entity.h"


#pragma once


#include "Entity.h"
#include "Direction.h"
#include "Subject.h"


class PacManModel : public Entity, public Subject {
private:
    int lives = 3;

public:
    PacManModel();
    void setDirection(Direction d);
    void setDesiredDirection(Direction d) { desiredDirection = d; }
    void update(double dt) override;


    void reset();
    int getLives() const { return lives; }
    void loseLife() { --lives; };
    bool isGameOver() const { return lives <= 0; }


};



#endif //PACMANGAME_PACMANMODEL_H