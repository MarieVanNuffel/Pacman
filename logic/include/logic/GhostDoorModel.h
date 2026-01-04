//
// Created by Marie Van Nuffel on 22/12/2025.
//

#ifndef PACMANGAME_GHOSTDOORMODEL_H
#define PACMANGAME_GHOSTDOORMODEL_H

#pragma once
#include "Entity.h"
#include "Subject.h"

namespace logic {
    class GhostDoorModel : public Entity, public Subject {
    public:
        GhostDoorModel() = default;
        ~GhostDoorModel() override = default;

        // Ghosts mogen enkel UIT spawn door de deur
        bool canGhostPass(double fromX, double fromY) const;

        // ✅ NIEUW: Check of ghost in de "deur zone" is (onder/bij deur)
        bool isGhostInDoorZone(double ghostX, double ghostY) const;

        // Check of ghost boven de deur zit
        bool aboveGhostDoor(double ghostX, double ghostY) const;

        void update(double) override {}
    };
}



#endif //PACMANGAME_GHOSTDOORMODEL_H