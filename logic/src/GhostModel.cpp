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
    speed = 2.0;
    mode = Mode::Waiting;

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
        speed = 2.0;
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
        } else {
            return;
        }
    }

    // ------------------------------------------------
    // 2) FEAR mode
    // ------------------------------------------------
    if (mode == Mode::Fear) {
        fearTimer += dt;
        if (fearTimer >= FEAR_DURATION) {
            setMode(Mode::Chase);
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
            setMode(Mode::Chase);
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
    // 3) CHASE / FEAR mode
    // ------------------------------------------------
    // a) LockedRandom
    // ------------------------------------------------
    Direction chosen;
    double snapX = std::floor(x) + 0.5;
    double snapY = std::floor(y) + 0.5;
    double snapEpsilon = 0.1;
    bool nearCenter = (std::abs(x - snapX) < snapEpsilon && std::abs(y - snapY) < snapEpsilon);

    // ✅ CHECK: Zijn we in de ghostdoor zone?
    bool nearDoor = false;
    for (const auto& door : worldRef->getGhostDoors()) {
        if (door->isGhostInDoorZone(x, y)) {
            nearDoor = true;
            break;
        }
    }

    // ✅ In deur zone: forceer UP
    if (nearDoor && nearCenter) {
        direction = Direction::UP;
        desiredDirection = Direction::UP;

        worldRef->tryMoveGhost(
            std::shared_ptr<Entity>(this, [](Entity*){}),
            Direction::UP,
            dt
        );
        return;
    }

    bool canMove = worldRef->canGhostMove(direction, x, y);

    // kijk of we in het midden van een tile zijn en een kruispunt
    bool atIntersection = false;
    if (nearCenter) {
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


    if (type == GhostType::LockedRandom) {
        // ✅ Normale LockedRandom logica (buiten deur zone)

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

        double bestDist = (mode == Mode::Fear) ? -1e9 : 1e9;
        std::vector<Direction> bestDirs;

        for (Direction d : finalDirs) {
            auto [ghostNextX, ghostNextY] = worldRef->predictStep(x, y, d);
            double dist = manhattan(ghostNextX, ghostNextY, targetX, targetY);

            // In fear mode: maximaliseer afstand
            if (mode == Mode::Fear) {
                if (dist > bestDist + 0.0001) {
                    bestDist = dist;
                    bestDirs.clear();
                    bestDirs.push_back(d);
                } else if (std::abs(dist - bestDist) < 0.0001) {
                    bestDirs.push_back(d);
                }
            }
            // In chase mode: minimaliseer afstand
            else {
                if (dist < bestDist - 0.0001) {
                    bestDist = dist;
                    bestDirs.clear();
                    bestDirs.push_back(d);
                } else if (std::abs(dist - bestDist) < 0.0001) {
                    bestDirs.push_back(d);
                }
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

            // In fear mode: maximaliseer afstand
            if (mode == Mode::Fear) {
                if (dist > bestDist + 0.0001) {
                    bestDist = dist;
                    bestDirs.clear();
                    bestDirs.push_back(d);
                } else if (std::abs(dist - bestDist) < 0.0001) {
                    bestDirs.push_back(d);
                }
            }
            // In chase mode: minimaliseer afstand
            else {
                if (dist < bestDist - 0.0001) {
                    bestDist = dist;
                    bestDirs.clear();
                    bestDirs.push_back(d);
                } else if (std::abs(dist - bestDist) < 0.0001) {
                    bestDirs.push_back(d);
                }
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