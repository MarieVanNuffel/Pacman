//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_GHOSTMODEL_H
#define PACMANGAME_GHOSTMODEL_H

#pragma once


#include "Entity.h"
#include "Direction.h"
#include "Subject.h"

class World;



class GhostModel : public Entity, public Subject {
public:
    enum class Mode { Waiting, Chase, Fear, Eaten };
    enum class GhostType {
        LockedRandom,     // vast in richting, soms random
        AheadOfPacman1,   // kijkt vóór Pac-Man
        AheadOfPacman2,   // hetzelfde, maar ander startmoment
        DirectChase       // jaagt rechtstreeks
    };
private:
    // TODO aanpassen voor verschillende ghosts: speed, direction
    GhostType type;
    Mode mode = Mode::Waiting;

    double startX = 0, startY = 0;
    double releaseTimer = 0.0;

    World *worldRef = nullptr;

    Direction decideDirection();

public:
    GhostModel(GhostType t);
    void update(double dt) override;

    void setWorld(World* w) { worldRef = w; };
    void setMode(Mode m);
    Mode getMode() const;
    GhostType getGhostType() const {return type;};


    void setStartPosition(double sx, double sy) { x = sx; y = sy; startX = sx; startY = sy; }

    Direction computeLockedDir(const World& world);

};



#endif //PACMANGAME_GHOSTMODEL_H