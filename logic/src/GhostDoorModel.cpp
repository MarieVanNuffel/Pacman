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
    // - Onder of BIJ de deur is (y >= doorY - 0.5)
    // - Horizontaal binnen 1 tile van de deur

    bool sameColumn = (ghostX == getX());
    bool inDoorTile = (ghostY == getY());
    bool underDoorTile = (ghostY == getY() + 1);

    return sameColumn && (inDoorTile || underDoorTile);
}
