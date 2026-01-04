//
// Created by Marie Van Nuffel on 22/12/2025.
//

#ifndef PACMANGAME_GHOSTDOORMODEL_H
#define PACMANGAME_GHOSTDOORMODEL_H

#pragma once
#include "Entity.h"
#include "Subject.h"

namespace logic {

    /**
     * @brief Model voor een ghostdoor
     *
     * De ghost door blokkeert (en pacman) ghosts behalve wanneer ze uit de spawn komen.
     */
    class GhostDoorModel : public Entity, public Subject {
    public:
        GhostDoorModel() = default;
        ~GhostDoorModel() override = default;

        /**
         * @brief Check of ghost door de deur mag passeren
         *
         * Ghost mag enkel erdoor als hij onder de deur zit en aligned is met de deur.
         *
         * @param ghostX X positie van ghost
         * @param ghostY Y positie van ghost
         * @return true als ghost door de deur mag
         */
        bool canGhostPass(double fromX, double fromY) const;

        /**
         * @brief Check of ghost in de deur zone zit
         *
         * Kijkt of ghost in de tile van de deur zit of de tile eronder.
         *
         * @param ghostX X positie van ghost
         * @param ghostY Y positie van ghost
         * @return true als ghost in of net onder de deur zit
         */
        bool isGhostInDoorZone(double ghostX, double ghostY) const;

        /**
         * @brief Check of ghost boven de deur zit
         *
         * Kijkt of ghost in de tile van de deur zit of de tile eronder.
         *
         * @param ghostX X positie van ghost
         * @param ghostY Y positie van ghost
         * @return true als ghost boven de deur zit
         */
        bool aboveGhostDoor(double ghostX, double ghostY) const;

        /**
         * @brief Update functie
         *
         * Ghost door is statisch, dus doet niets
         */
        void update(double) override {}
    };
}



#endif //PACMANGAME_GHOSTDOORMODEL_H