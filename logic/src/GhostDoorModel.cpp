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
    int ghostTileX = static_cast<int>(std::floor(ghostX));
    int ghostTileY = static_cast<int>(std::floor(ghostY));

    int doorTileX = static_cast<int>(std::floor(getX()));
    int doorTileY = static_cast<int>(std::floor(getY()));

    bool sameColumn = (ghostTileX == doorTileX);
    bool inDoorTile = (ghostTileY == doorTileY);
    bool underDoorTile = (ghostTileY == doorTileY + 1);

    return sameColumn && (inDoorTile || underDoorTile);
}

bool GhostDoorModel::aboveGhostDoor(double ghostX, double ghostY) const {
    int ghostTileX = static_cast<int>(std::floor(ghostX));
    int ghostTileY = static_cast<int>(std::floor(ghostY));

    int doorTileX = static_cast<int>(std::floor(getX()));
    int doorTileY = static_cast<int>(std::floor(getY()));

    bool sameColumn = (ghostTileX == doorTileX);
    bool aboveDoorTile = (ghostTileY == doorTileY - 1);

    return sameColumn && aboveDoorTile;
}
