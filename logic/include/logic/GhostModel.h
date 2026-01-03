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
    GhostType type;
    Mode mode;
    double chaseSpeed;

    double startX = 0, startY = 0;
    double releaseTimer = 0.0;

    double decisionTimer = 0.0;           // tijd sinds laatste richting-keuze
    double decisionCooldown = 0.12;      // seconden om een keuze vast te houden

    // Fear mode
    double fearTimer = 0.0;
    double fearDuration = 6.0;

    World *worldRef = nullptr;

    Direction decideDirection();


public:
    GhostModel(GhostType t);
    void update(double dt) override;

    void setWorld(World* w) { worldRef = w; };
    void setMode(Mode m);
    void setSpeed(double s) { speed = s;};
    void setFearDuration(double d) { fearDuration = d; };
    void setChaseSpeed(double d) {chaseSpeed = d; }
    Mode getMode() const;
    GhostType getGhostType() const {return type; };
    double getStartX() const {return startX; };
    double getStartY() const {return startY; };
    double getFearDuration() const {return fearDuration; };
    double getChaseSpeed() const {return chaseSpeed; }
    void setReleaseTimer(double dt) { releaseTimer = dt; };


    void setStartPosition(double sx, double sy) { x = sx; y = sy; startX = sx; startY = sy; }

    Direction computeLockedDir(const World& world);

};



#endif //PACMANGAME_GHOSTMODEL_H