#include "logic/GhostModel.h"
#include "logic/World.h"
#include "logic/Random.h"
#include <cmath>
#include <iostream>

// Manhattan afstand helper
static double manhattan(double x1, double y1, double x2, double y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

GhostModel::GhostModel(GhostType t) : type(t) {
    speed = 2.8;
    mode = Mode::Waiting;

    if (type == GhostType::AheadOfPacman2 || type == GhostType::DirectChase) {
        desiredDirection = Direction::LEFT;
        direction = Direction::LEFT;
    } else {
        desiredDirection = Direction::UP;
        direction = Direction::UP;
    }
}

void GhostModel::setMode(Mode m) {
    mode = m;

    if (m == Mode::Fear) {
        speed = 1.5;
        switch (desiredDirection) {
            case Direction::UP:    desiredDirection = Direction::DOWN; break;
            case Direction::DOWN:  desiredDirection = Direction::UP; break;
            case Direction::LEFT:  desiredDirection = Direction::RIGHT; break;
            case Direction::RIGHT: desiredDirection = Direction::LEFT; break;
            default: break;
        }
        direction = desiredDirection;
    }

    if (m == Mode::Eaten) {
        speed = 3.0;
    }

    if (m == Mode::Chase) {
        speed = 2.8;
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

    // ------------------------------------------------
    // 1) WAITING MODE
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
            desiredDirection = Direction::UP;
            direction = Direction::UP;
        } else {
            return;
        }
    }

    // ------------------------------------------------
    // 2) EATEN → terug naar spawn
    // ------------------------------------------------
    if (mode == Mode::Eaten) {
        double dx = startX - x;
        double dy = startY - y;
        double len = std::abs(dx) + std::abs(dy);

        if (len > 0.001) {
            x += (dx / len) * speed * dt;
            y += (dy / len) * speed * dt;
        }

        if (std::abs(x - startX) < 0.05 &&
            std::abs(y - startY) < 0.05) {
            x = startX;
            y = startY;
            mode = Mode::Waiting;
            releaseTimer = 0.0;

            if (type == GhostType::AheadOfPacman2 || type == GhostType::DirectChase) {
                desiredDirection = Direction::LEFT;
                direction = Direction::LEFT;
            } else {
                desiredDirection = Direction::UP;
                direction = Direction::UP;
            }
            }
        return;
    }

    // ------------------------------------------------
    // 3) CHASE MODE
    // ------------------------------------------------
    // a) LockedRandom
    // ------------------------------------------------
    Direction chosen;
    double snapX = std::floor(x) + 0.5;
    double snapY = std::floor(y) + 0.5;

    // ✅ CHECK: Zijn we in de ghostdoor zone?
    bool nearDoor = false;
    for (const auto& door : worldRef->getGhostDoors()) {
        if (door->isGhostInDoorZone(x, y)) {
            nearDoor = true;
            break;
        }
    }

    // ✅ In deur zone: forceer UP
    if (nearDoor) {
        direction = Direction::UP;
        desiredDirection = Direction::UP;

        worldRef->tryMoveGhost(
            std::shared_ptr<Entity>(this, [](Entity*){}),
            Direction::UP,
            dt
        );
        return;
    }

    if (type == GhostType::LockedRandom) {
        // ✅ Normale LockedRandom logica (buiten deur zone)
        bool canMove = worldRef->canGhostMove(direction, x, y);
        bool exactlyOnCenter = (std::abs(x - snapX) < 0.01 && std::abs(y - snapY) < 0.01);

        bool atIntersection = false;
        if (exactlyOnCenter) {
            atIntersection = worldRef->isIntersection(x, y);
        }

        if (canMove && !atIntersection) {
            worldRef->tryMoveGhost(
                std::shared_ptr<Entity>(this, [](Entity*){}),
                direction,
                dt
            );
            return;
        }

        if (!canMove) {
            if (direction == Direction::UP || direction == Direction::DOWN) {
                y = snapY;
            } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
                x = snapX;
            }
            setPosition(x, y);
        }

        std::vector<Direction> viableDirs = worldRef->getFreeDirections(x, y);

        if (viableDirs.empty()) {
            return;
        }

        Direction chosen;

        if (logic::Random::instance().nextDouble(0.0, 1.0) < 0.5) {
            chosen = viableDirs[
                logic::Random::instance().nextInt(0, static_cast<int>(viableDirs.size()) - 1)
            ];
        } else {
            bool found = false;
            for (Direction d : viableDirs) {
                if (d == direction) {
                    chosen = direction;
                    found = true;
                    break;
                }
            }
            if (!found) {
                chosen = viableDirs[
                    logic::Random::instance().nextInt(0, static_cast<int>(viableDirs.size()) - 1)
                ];
            }
        }

        direction = chosen;
        desiredDirection = chosen;

        worldRef->tryMoveGhost(
            std::shared_ptr<Entity>(this, [](Entity*){}),
            direction,
            dt
        );
        return;
    }

    // ------------------------------------------------
    // 4) AheadOfPacman & DirectChase: ALTIJD herberekenen
    // ------------------------------------------------

    auto pm = worldRef->getPacman();
    std::vector<Direction> finalDirs = worldRef->getFreeDirections(x,y);

    // ------------------------------------------------
    // AheadOfPacman: minimaliseer afstand naar vóór Pac-Man
    // ------------------------------------------------
    if (type == GhostType::AheadOfPacman1 ||
        type == GhostType::AheadOfPacman2) {

        auto [targetX, targetY] = worldRef->predictStep(
            pm->getX(), pm->getY(), pm->getDirection()
        );

        double bestDist = 1e9;
        std::vector<Direction> bestDirs;

        for (Direction d : finalDirs) {
            auto [ghostNextX, ghostNextY] = worldRef->predictStep(x, y, d);
            double dist = manhattan(ghostNextX, ghostNextY, targetX, targetY);

            if (dist < bestDist - 0.0001) {
                bestDist = dist;
                bestDirs.clear();
                bestDirs.push_back(d);
            }
            else if (std::abs(dist - bestDist) < 0.0001) {
                bestDirs.push_back(d);
            }
        }

        if (!bestDirs.empty()) {
            chosen = bestDirs[
                logic::Random::instance().nextInt(0, static_cast<int>(bestDirs.size()) - 1)
            ];
        }
    }

    // ------------------------------------------------
    // DirectChase: minimaliseer afstand naar Pac-Man
    // ------------------------------------------------
    else if (type == GhostType::DirectChase) {

        double bestDist = 1e9;
        std::vector<Direction> bestDirs;

        for (Direction d : finalDirs) {
            auto [ghostNextX, ghostNextY] = worldRef->predictStep(x, y, d);
            double dist = manhattan(ghostNextX, ghostNextY, pm->getX(), pm->getY());

            if (dist < bestDist - 0.0001) {
                bestDist = dist;
                bestDirs.clear();
                bestDirs.push_back(d);
            }
            else if (std::abs(dist - bestDist) < 0.0001) {
                bestDirs.push_back(d);
            }
        }

        if (!bestDirs.empty()) {
            chosen = bestDirs[
                logic::Random::instance().nextInt(0, static_cast<int>(bestDirs.size()) - 1)
            ];
        }
    }

    // ------------------------------------------------
    // 5) Update richting en beweeg
    // ------------------------------------------------
    desiredDirection = chosen;
    direction = chosen;

    worldRef->tryMoveGhost(
        std::shared_ptr<Entity>(this, [](Entity*){}),
        direction,
        dt
    );
}

// void GhostModel::update(double dt) {
//     if (!worldRef) return;
//
//     releaseTimer += dt;
//
//     // -------------------------------
//     // 1) WAITING MODE
//     // -------------------------------
//     if (mode == Mode::Waiting) {
//         bool shouldRelease = false;
//
//         switch (type) {
//             case GhostType::LockedRandom:
//             case GhostType::AheadOfPacman1:
//                 shouldRelease = true;
//                 break;
//             case GhostType::AheadOfPacman2:
//                 shouldRelease = (releaseTimer >= 5.0);
//                 break;
//             case GhostType::DirectChase:
//                 shouldRelease = (releaseTimer >= 10.0);
//                 break;
//         }
//
//         if (!shouldRelease) return;
//
//         mode = Mode::Chase;
//         direction = desiredDirection = Direction::UP;
//     }
//
//     // -------------------------------
//     // 2) EATEN → terug naar spawn
//     // -------------------------------
//     if (mode == Mode::Eaten) {
//         double dx = startX - x;
//         double dy = startY - y;
//         double len = std::abs(dx) + std::abs(dy);
//
//         if (len > 0.001) {
//             x += (dx / len) * speed * dt;
//             y += (dy / len) * speed * dt;
//             setPosition(x, y);
//         }
//
//         if (std::abs(x - startX) < 0.05 && std::abs(y - startY) < 0.05) {
//             setPosition(startX, startY);
//             mode = Mode::Waiting;
//             releaseTimer = 0.0;
//
//             direction = desiredDirection =
//                 (type == GhostType::AheadOfPacman2 || type == GhostType::DirectChase)
//                 ? Direction::LEFT
//                 : Direction::UP;
//         }
//         return;
//     }
//
//     // -------------------------------
//     // 3) UNDER DOOR LOGIC
//     // -------------------------------
//     bool underDoor = false;
//     double snapX = std::floor(x) + 0.5;
//     double snapY = std::floor(y) + 0.5;
//
//     for (auto& door : worldRef->getGhostDoors()) {
//         if (y > door->getY() && std::abs(x - door->getX()) < 0.49) {
//             underDoor = true;
//             direction = desiredDirection = Direction::UP;
//
//             // Snap naar center van tile voor correcte movement
//             setPosition(snapX, snapY);
//
//             worldRef->tryMoveGhost(
//                 std::shared_ptr<Entity>(this, [](Entity*){}),
//                 Direction::UP,
//                 dt
//             );
//             return;
//         }
//     }
//
//     // -------------------------------
//     // 4) LOCKED RANDOM
//     // -------------------------------
//     if (type == GhostType::LockedRandom) {
//         // Snap naar center als dichtbij
//         if (std::abs(x - snapX) < 0.15 && std::abs(y - snapY) < 0.15) {
//             x = snapX; y = snapY;
//             setPosition(x, y);
//         }
//
//         bool canMove = worldRef->canGhostMove(direction, x, y);
//
//         bool onCenter = (std::abs(x - snapX) < 0.01 && std::abs(y - snapY) < 0.01);
//
//         if (canMove && !onCenter) {
//             worldRef->tryMoveGhost(
//                 std::shared_ptr<Entity>(this, [](Entity*){}),
//                 direction,
//                 dt
//             );
//             return;
//         }
//
//         if (!canMove && onCenter) {
//             setPosition(snapX, snapY);
//         }
//
//         auto dirs = worldRef->getFreeDirections(x, y);
//         if (dirs.empty()) return;
//
//         Direction chosen;
//         if (logic::Random::instance().nextDouble(0.0, 1.0) < 0.5) {
//             chosen = dirs[logic::Random::instance().nextInt(0, dirs.size()-1)];
//         } else {
//             chosen = (std::find(dirs.begin(), dirs.end(), direction) != dirs.end())
//                      ? direction
//                      : dirs[logic::Random::instance().nextInt(0, dirs.size()-1)];
//         }
//
//         direction = desiredDirection = chosen;
//         worldRef->tryMoveGhost(
//             std::shared_ptr<Entity>(this, [](Entity*){}),
//             direction,
//             dt
//         );
//         return;
//     }
//
//     // -------------------------------
//     // 5) AHEAD / DIRECT CHASE
//     // -------------------------------
//     auto pm = worldRef->getPacman();
//
//     double cx = x, cy = y;
//     if (std::abs(x - snapX) < 0.15 && std::abs(y - snapY) < 0.15) {
//         cx = snapX; cy = snapY; // snap naar tile center
//     }
//
//     auto dirs = worldRef->getFreeDirections(cx, cy);
//     if (dirs.empty()) return;
//
//     Direction chosen = direction;
//     double bestDist = 1e9;
//     std::vector<Direction> bestDirs;
//
//     for (Direction d : dirs) {
//         auto [nx, ny] = worldRef->predictStep(cx, cy, d);
//         double targetX, targetY;
//
//         if (type == GhostType::DirectChase) {
//             targetX = pm->getX();
//             targetY = pm->getY();
//         } else { // AheadOfPacman
//             std::tie(targetX, targetY) = worldRef->predictStep(pm->getX(), pm->getY(), pm->getDirection());
//         }
//
//         double dist = std::abs(nx - targetX) + std::abs(ny - targetY);
//
//         if (dist < bestDist - 0.001) {
//             bestDist = dist;
//             bestDirs = {d};
//         } else if (std::abs(dist - bestDist) < 0.001) {
//             bestDirs.push_back(d);
//         }
//     }
//
//     if (!bestDirs.empty()) {
//         chosen = bestDirs[logic::Random::instance().nextInt(0, bestDirs.size()-1)];
//     }
//
//     // Snap naar center voordat we bewegen
//     if (std::abs(x - snapX) < 0.15 && std::abs(y - snapY) < 0.15) {
//         setPosition(snapX, snapY);
//     }
//
//     direction = desiredDirection = chosen;
//     worldRef->tryMoveGhost(
//         std::shared_ptr<Entity>(this, [](Entity*){}),
//         direction,
//         dt
//     );
// }
//
//
