#include "logic/GhostModel.h"
#include "logic/World.h"
#include "logic/Random.h"
#include <cmath>
#include <iostream>

// Manhattan afstand helper
static double manhattan(double x1, double y1, double x2, double y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

static bool isOpposite(Direction a, Direction b) {
    if (a == Direction::UP && b == Direction::DOWN) return true;
    if (a == Direction::DOWN && b == Direction::UP) return true;
    if (a == Direction::LEFT && b == Direction::RIGHT) return true;
    if (a == Direction::RIGHT && b == Direction::LEFT) return true;
    return false;
}

GhostModel::GhostModel(GhostType t) : type(t) {
    chaseSpeed = 2.0;
    speed = chaseSpeed;
    mode = Mode::Waiting;
    // decisionTimer en decisionCooldown kunnen ook hier worden geïnitialiseerd als je dat wilt:
    decisionTimer = decisionCooldown; // start met cooldown voldaan zodat eerste beslissing meteen mag

    if (type == GhostType::AheadOfPacman2 || type == GhostType::DirectChase) {
        desiredDirection = Direction::LEFT;
        direction = Direction::LEFT;
    } else {
        desiredDirection = Direction::RIGHT;
        direction = Direction::RIGHT;
    }
}

void GhostModel::setMode(Mode m) {
    mode = m;

    if (m == Mode::Fear) {
        speed = 1.5;
        fearTimer = 0.0;

        // ✅ Reverse direction
        switch (direction) {
            case Direction::UP:    direction = Direction::DOWN; break;
            case Direction::DOWN:  direction = Direction::UP; break;
            case Direction::LEFT:  direction = Direction::RIGHT; break;
            case Direction::RIGHT: direction = Direction::LEFT; break;
            default: break;
        }
        desiredDirection = direction;

        // Snap naar center als we dichtbij zijn
        if (worldRef) {
            double snapX = std::floor(x) + 0.5;
            double snapY = std::floor(y) + 0.5;
            if (std::abs(x - snapX) < 0.2 && std::abs(y - snapY) < 0.2) {
                setPosition(snapX, snapY);
            }
        }
    }

    if (m == Mode::Eaten) {
        speed = 3.0;
    }
}

GhostModel::Mode GhostModel::getMode() const {
    return mode;
}

Direction GhostModel::decideDirection() {
    int r = logic::Random::instance().nextInt(0, 3);
    switch (r) {
        case 0: return Direction::UP;
        case 1: return Direction::DOWN;
        case 2: return Direction::LEFT;
        default: return Direction::RIGHT;
    }
}

void GhostModel::update(double dt) {
    if (!worldRef) return;

    releaseTimer += dt;
    decisionTimer += dt; // update cooldown timer

    // ------------------------------------------------
    // 1) WAITING mode
    // ------------------------------------------------
    if (mode == Mode::Waiting) {
        bool shouldRelease = false;

        switch (type) {
            case GhostType::LockedRandom:
            case GhostType::AheadOfPacman1:
                shouldRelease = true;
                break;
            case GhostType::AheadOfPacman2:
                shouldRelease = (releaseTimer >= 5.0);
                break;
            case GhostType::DirectChase:
                shouldRelease = (releaseTimer >= 10.0);
                break;
        }

        if (shouldRelease) {
            mode = Mode::Chase;
            // allow immediate decision after release
            decisionTimer = decisionCooldown;
        } else {
            return;
        }
    }

    // ------------------------------------------------
    // 2) FEAR mode
    // ------------------------------------------------
    if (mode == Mode::Fear) {
        fearTimer += dt;
        if (fearTimer >= fearDuration) {
            setMode(Mode::Chase);
            return;
        }
    }

    // ------------------------------------------------
    // 3) EATEN → terug naar spawn
    //    -> MINIMALE AANPASSING: gebruik exact dezelfde per-tile logica als chase, maar
    //       doel = spawn tile, en bepaal beste richting via breadth-first (World::findPath)
    //       met allowDoor = true, en verplaats ALTIJD via tryMoveGhost (geen directe x/y!)
    // ------------------------------------------------
     if (mode == Mode::Eaten) {
        double snapX = std::floor(x) + 0.5;
        double snapY = std::floor(y) + 0.5;
        double snapEpsilon = 0.15;
        bool nearCenter = (std::abs(x - snapX) < snapEpsilon && std::abs(y - snapY) < snapEpsilon);

        // ✅ Corridor centering: snap orthogonale richting
        double centerForceEpsilon = 0.20;
        if (direction == Direction::UP || direction == Direction::DOWN) {
            if (std::abs(x - snapX) < centerForceEpsilon) { x = snapX; setPosition(x, y); }
        } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
            if (std::abs(y - snapY) < centerForceEpsilon) { y = snapY; setPosition(x, y); }
        }

        // gebruik gecentreerde coords voor BFS
        double currentX = x, currentY = y;
        if (nearCenter) {
            currentX = snapX;
            currentY = snapY;
        }

        // compute tile coords for BFS
        int csx = static_cast<int>(std::floor(currentX));
        int csy = static_cast<int>(std::floor(currentY));
        int tx = static_cast<int>(std::floor(startX));
        int ty = static_cast<int>(std::floor(startY));

         // boven de ghostdoor moet je naar beneden (specifiek deze map)
         if (direction != Direction::DOWN) {
             for (const auto& door : worldRef->getGhostDoors()) {
                 if (nearCenter && door->aboveGhostDoor(snapX, snapY)) {
                     direction = Direction::DOWN;
                     desiredDirection = Direction::DOWN;
                     worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), Direction::DOWN, dt);
                     return;
                 }
             }
         }

        // If we are already basically at spawn center, finish eaten
        if (std::abs(x - startX) < 0.05 && std::abs(y - startY) < 0.05) {
            x = startX;
            y = startY;
            setPosition(x, y);
            setMode(Mode::Chase);
            setSpeed(chaseSpeed);
            releaseTimer = 0.0;

            if (type == GhostType::AheadOfPacman2 || type == GhostType::DirectChase) {
                desiredDirection = Direction::LEFT;
                direction = Direction::LEFT;
            } else {
                desiredDirection = Direction::UP;
                direction = Direction::UP;
            }
            return;
        }

        // ✅ Alleen nieuwe richting kiezen als we near center zijn (bij intersection)
        // Check of we door kunnen bewegen (ghostdoor moet passeerbaar zijn in eaten mode)
        bool canMove = worldRef->canGhostMove(direction, x, y);

        // ✅ Als we geblokkeerd zijn door ghostdoor, kijk of we erin mogen
        if (!canMove) {
            double checkStep = 0.05;
            double testX = x, testY = y;
            switch (direction) {
                case Direction::UP:    testY -= checkStep; break;
                case Direction::DOWN:  testY += checkStep; break;
                case Direction::LEFT:  testX -= checkStep; break;
                case Direction::RIGHT: testX += checkStep; break;
                default: break;
            }
        }

        bool atIntersection = nearCenter && worldRef->isIntersection(snapX, snapY);

        // Als we gewoon rechtdoor kunnen en niet bij een kruising zijn, ga door
        if (canMove && !atIntersection) {
            worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
            return;
        }

        // Als geblokkeerd, snap as naar center
        if (!canMove) {
            if (direction == Direction::UP || direction == Direction::DOWN) y = snapY;
            else if (direction == Direction::LEFT || direction == Direction::RIGHT) x = snapX;
            setPosition(x, y);
        }

        // Ask world for BFS path (allowing ghostdoor)
        std::vector<Direction> path;
        if (worldRef) path = worldRef->findPath(csx, csy, tx, ty, true);

        if (!path.empty()) {
            Direction cand = path.front();

            // avoid immediate reverse unless that's the only option
            if (isOpposite(cand, direction) && path.size() > 1) {
                std::vector<Direction> viable = worldRef->getFreeDirections(currentX, currentY);
                for (Direction alt : viable) {
                    if (!isOpposite(alt, direction)) { cand = alt; break; }
                }
            }

            desiredDirection = cand;
            direction = cand;
            worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
        } else {
            // BFS failed: continue in current direction if possible
            if (worldRef->canGhostMove(direction, x, y)) {
                worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
            }
        }
        return;
    }

    // ------------------------------------------------
    // 4) CHASE / FEAR mode core (unchanged aside from minimal centering / cooldown)
    // ------------------------------------------------
    Direction chosen = Direction::NONE;
    double snapX = std::floor(x) + 0.5;
    double snapY = std::floor(y) + 0.5;

    // iets ruimere marge zodat we beslissingen alleen maken wanneer quasi- gecentreerd
    double snapEpsilon = 0.15;
    bool nearCenter = (std::abs(x - snapX) < snapEpsilon && std::abs(y - snapY) < snapEpsilon);

    // minimale corridor-centering (orthogonale coord) — voorkomt dat ghosts buiten het midden blijven lopen
    double centerForceEpsilon = 0.20;
    if (direction == Direction::UP || direction == Direction::DOWN) {
        if (std::abs(x - snapX) < centerForceEpsilon) { x = snapX; setPosition(x, y); }
    } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
        if (std::abs(y - snapY) < centerForceEpsilon) { y = snapY; setPosition(x, y); }
    }

    // check ghostdoor
    bool nearDoor = false;
    for (const auto& door : worldRef->getGhostDoors()) {
        if (door->isGhostInDoorZone(x, y)) { nearDoor = true; break; }
    }
    if (nearDoor && nearCenter) {
        direction = Direction::UP;
        desiredDirection = Direction::UP;
        // reset cooldown so we don't immediately switch again
        decisionTimer = 0.0;
        worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), Direction::UP, dt);
        return;
    }

    bool canMove = worldRef->canGhostMove(direction, x, y);

    // intersection check uses centered coords when nearCenter
    bool atIntersection = false;
    if (nearCenter) atIntersection = worldRef->isIntersection(snapX, snapY);

    // If moving fine and not at an intersection, continue
    if (canMove && !atIntersection) {
        worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
        return;
    }

    // If current direction is blocked, snap axis to center to avoid corner stuck and allow immediate decision
    if (!canMove) {
        if (direction == Direction::UP || direction == Direction::DOWN) {
            y = snapY;
        } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
            x = snapX;
        }
        setPosition(x, y);
        // set decisionTimer high so we will evaluate options below even if cooldown not passed
        decisionTimer = decisionCooldown;
    }

    // NEW: if we're at an intersection but still inside cooldown, don't re-evaluate: continue straight
    // This prevents immediate flip-flop between two directions.
    if (atIntersection && decisionTimer < decisionCooldown) {
        worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
        return;
    }

    // gebruik gecentreerde coords voor vrije richtingen wanneer nearCenter is
    double freeCheckX = x, freeCheckY = y;
    if (nearCenter) { freeCheckX = snapX; freeCheckY = snapY; }

    std::vector<Direction> viableDirs = worldRef->getFreeDirections(freeCheckX, freeCheckY);
    if (viableDirs.empty()) { return; }

    bool allowReverse = (mode == Mode::Fear || mode == Mode::Eaten);

    // LockedRandom: kies richting (vermijd reverse als mogelijk)
    if (type == GhostType::LockedRandom) {
        if (logic::Random::instance().nextDouble(0.0, 1.0) < 0.5) {
            Direction cand = viableDirs[
                logic::Random::instance().nextInt(0, static_cast<int>(viableDirs.size()) - 1)
            ];
            if (isOpposite(cand, direction) && viableDirs.size() > 1) {
                std::vector<Direction> nonOpp;
                for (Direction d : viableDirs) if (!isOpposite(d, direction)) nonOpp.push_back(d);
                if (!nonOpp.empty()) {
                    cand = nonOpp[logic::Random::instance().nextInt(0, static_cast<int>(nonOpp.size()) - 1)];
                }
            }
            chosen = cand;
        } else {
            bool found = false;
            for (Direction d : viableDirs) {
                if (d == direction) { chosen = direction; found = true; break; }
            }
            if (!found) {
                Direction cand = viableDirs[
                    logic::Random::instance().nextInt(0, static_cast<int>(viableDirs.size()) - 1)
                ];
                if (isOpposite(cand, direction) && viableDirs.size() > 1) {
                    std::vector<Direction> nonOpp;
                    for (Direction d : viableDirs) if (!isOpposite(d, direction)) nonOpp.push_back(d);
                    if (!nonOpp.empty()) {
                        cand = nonOpp[logic::Random::instance().nextInt(0, static_cast<int>(nonOpp.size()) - 1)];
                    }
                }
                chosen = cand;
            }
        }

        if (chosen != Direction::NONE) {
            desiredDirection = chosen;
            direction = chosen;
            // reset cooldown after a deliberate choice
            decisionTimer = 0.0;

            worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
        }
        return;
    }

    // Ahead/Direct chase: always herberekenen (maar nog steeds met cooldown guard hierboven)
    auto pm = worldRef->getPacman();
    double currentX = x, currentY = y;
    if (std::abs(x - snapX) < 0.15 && std::abs(y - snapY) < 0.15) { currentX = snapX; currentY = snapY; }

    std::vector<Direction> finalDirs = worldRef->getFreeDirections(currentX, currentY);

    if (type == GhostType::AheadOfPacman1 || type == GhostType::AheadOfPacman2) {
        auto [targetX, targetY] = worldRef->predictStep(pm->getX(), pm->getY(), pm->getDirection());
        double bestDist = 1e9;
        std::vector<Direction> bestDirs;
        for (Direction d : finalDirs) {
            auto [ghostNextX, ghostNextY] = worldRef->predictStep(currentX, currentY, d);
            double dist = manhattan(ghostNextX, ghostNextY, targetX, targetY);
            if (dist < bestDist - 0.0001) { bestDist = dist; bestDirs.clear(); bestDirs.push_back(d); }
            else if (std::abs(dist - bestDist) < 0.0001) bestDirs.push_back(d);
        }
        if (!bestDirs.empty()) {
            Direction cand = bestDirs[logic::Random::instance().nextInt(0, static_cast<int>(bestDirs.size()) - 1)];
            if (isOpposite(cand, direction) && bestDirs.size() > 1) {
                std::vector<Direction> nonOpp;
                for (Direction d : bestDirs) if (!isOpposite(d, direction)) nonOpp.push_back(d);
                if (!nonOpp.empty()) cand = nonOpp[logic::Random::instance().nextInt(0, static_cast<int>(nonOpp.size()) - 1)];
            }
            chosen = cand;
        }
    } else if (type == GhostType::DirectChase) {
        double bestDist = 1e9;
        std::vector<Direction> bestDirs;
        for (Direction d : finalDirs) {
            auto [ghostNextX, ghostNextY] = worldRef->predictStep(currentX, currentY, d);
            double dist = manhattan(ghostNextX, ghostNextY, pm->getX(), pm->getY());
            if (dist < bestDist - 0.0001) { bestDist = dist; bestDirs.clear(); bestDirs.push_back(d); }
            else if (std::abs(dist - bestDist) < 0.0001) bestDirs.push_back(d);
        }
        if (!bestDirs.empty()) {
            Direction cand = bestDirs[logic::Random::instance().nextInt(0, static_cast<int>(bestDirs.size()) - 1)];
            if (isOpposite(cand, direction) && bestDirs.size() > 1) {
                std::vector<Direction> nonOpp;
                for (Direction d : bestDirs) if (!isOpposite(d, direction)) nonOpp.push_back(d);
                if (!nonOpp.empty()) cand = nonOpp[logic::Random::instance().nextInt(0, static_cast<int>(nonOpp.size()) - 1)];
            }
            chosen = cand;
        }
    }

    if (chosen != Direction::NONE) {
        desiredDirection = chosen;
        direction = chosen;
        decisionTimer = 0.0; // reset cooldown on new decision
        worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
    } else {
        worldRef->tryMoveGhost(std::shared_ptr<Entity>(this, [](Entity*){}), direction, dt);
    }
}