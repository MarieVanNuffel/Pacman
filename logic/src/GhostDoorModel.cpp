//
// Created by Marie Van Nuffel on 22/12/2025.
//

#include "../include/logic/GhostDoorModel.h"
#include <cmath>


bool GhostDoorModel::canGhostPass(double ghostX, double ghostY) const {
    // Ghost mag alleen passeren als hij onder de deur zit
    bool isBelow = ghostY > getY();
    bool isAligned = std::abs(ghostX - getX()) < 0.49;

    return isBelow && isAligned;
}

bool GhostDoorModel::isGhostInDoorZone(double ghostX, double ghostY) const {
    // Ghost is in de deur zone als hij:
    // - Onder of IN de deur is (y >= doorY - 0.5)

    bool isInVerticalZone = ghostY >= getY() - 0.5;

    return isInVerticalZone;
}
