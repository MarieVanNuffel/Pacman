#include "logic/GhostModel.h"
#include "logic/World.h"
#include "logic/Random.h"
#include <cmath>
#include <iostream>

namespace logic {
// Manhattan afstand helper
static double manhattan(double x1, double y1, double x2, double y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// Check of twee richtingen elkaars tegenovergestelde zijn
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

        decisionTimer = decisionCooldown; // ghost mag direct kiezen

        // Start Direction afhankelijk van ghost type (ghosts kijken naar elkaar in box)
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

            // direction wordt omgekeerd bij FEAR mode
            switch (direction) {
                case Direction::UP:    direction = Direction::DOWN; break;
                case Direction::DOWN:  direction = Direction::UP; break;
                case Direction::LEFT:  direction = Direction::RIGHT; break;
                case Direction::RIGHT: direction = Direction::LEFT; break;
                default: break;
            }
            desiredDirection = direction;
        }

        if (m == Mode::Eaten) {
            speed = 4.0; // snel terug naar spawn
        }
    }


    void GhostModel::update(double dt) {
        auto worldPtr = worldRef.lock();
        if (!worldPtr) return;
        if (frozen) return; // geen beweging of updates timers wanneer bevroren

        releaseTimer += dt;
        decisionTimer += dt; // update cooldown timer

        // --- WAITING mode ---
        if (mode == Mode::Waiting) {
            bool shouldRelease = false;

            switch (type) { // afhankelijk van ghost type, moet die langer wachten
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
                decisionTimer = decisionCooldown; // direct richting kiezen
            } else {
                return;
            }
        }

        // --- FEAR mode ---
        if (mode == Mode::Fear) {
            fearTimer += dt;
            if (fearTimer >= fearDuration) {
                setMode(Mode::Chase); // terug naar chase
                return;
            }
        }

        // --- EATEN mode ---
        if (mode == Mode::Eaten) {
            double snapX = std::floor(x) + 0.5;
            double snapY = std::floor(y) + 0.5;
            double snapEpsilon = 0.15;
            bool nearCenter = (std::abs(x - snapX) < snapEpsilon && std::abs(y - snapY) < snapEpsilon);

            // Zorgen dat de ghosts in het midden van de gang lopen
            double centerForceEpsilon = 0.20;
            if (direction == Direction::UP || direction == Direction::DOWN) {
                if (std::abs(x - snapX) < centerForceEpsilon) { x = snapX; setPosition(x, y); }
            } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
                if (std::abs(y - snapY) < centerForceEpsilon) { y = snapY; setPosition(x, y); }
            }

            // Gecentreerde coords voor breadth first
            double currentX = x, currentY = y;
            if (nearCenter) {
                currentX = snapX;
                currentY = snapY;
            }

            // grid coords for breadth first
            int csx = static_cast<int>(std::floor(currentX));
            int csy = static_cast<int>(std::floor(currentY));
            int tx = static_cast<int>(std::floor(startX));
            int ty = static_cast<int>(std::floor(startY));

            // boven de ghostdoor moet je naar beneden (specifiek deze map)
            if (direction != Direction::DOWN) {
                for (const auto& door : worldPtr->getGhostDoors()) {
                    if (nearCenter && door->aboveGhostDoor(snapX, snapY)) {
                        direction = Direction::DOWN;
                        desiredDirection = Direction::DOWN;
                        worldPtr->tryMoveGhost(this, Direction::DOWN, dt);
                        return;
                    }
                }
            }

            // Aangekomen bij spawnpoint, terug naar chase
            if (std::abs(x - startX) < 0.05 && std::abs(y - startY) < 0.05) {
                x = startX;
                y = startY;
                setPosition(x, y);
                setMode(Mode::Chase);
                setSpeed(chaseSpeed);
                releaseTimer = 0.0;

                // Direction vanaf dat ze op hun spawn positie zijn
                if (type == GhostType::AheadOfPacman2 || type == GhostType::DirectChase) {
                    desiredDirection = Direction::LEFT;
                    direction = Direction::LEFT;
                } else {
                    desiredDirection = Direction::UP;
                    direction = Direction::UP;
                }
                return;
            }

            bool canMove = worldPtr->canGhostMove(direction, x, y);
            bool atIntersection = nearCenter && worldPtr->isIntersection(snapX, snapY);

            // Gewoon rechtdoor gaan als ze niet bij een kruispunt zijn (als het kan)
            if (canMove && !atIntersection) {
                worldPtr->tryMoveGhost(this, direction, dt);
                return;
            }

            // Als ze geblokkeerd zijn, snap naar het midden
            if (!canMove) {
                if (direction == Direction::UP || direction == Direction::DOWN) y = snapY;
                else if (direction == Direction::LEFT || direction == Direction::RIGHT) x = snapX;
                setPosition(x, y);
            }

            // pad wordt berekent via breadth first search
            std::vector<Direction> path;
            if (worldPtr) path = worldPtr->findPath(csx, csy, tx, ty, true); // ghostdoor is toegestaan

            if (!path.empty()) {
                Direction cand = path.front();

                // Liefst niet 180 graden draaien
                if (isOpposite(cand, direction) && path.size() > 1) {
                    std::vector<Direction> viable = worldPtr->getFreeDirections(currentX, currentY);
                    for (Direction alt : viable) {
                        if (!isOpposite(alt, direction)) { cand = alt; break; }
                    }
                }

                desiredDirection = cand;
                direction = cand;
                worldPtr->tryMoveGhost(this, direction, dt);
            } else {
                // Als het zou falen, blijf huidige direction volgen
                if (worldPtr->canGhostMove(direction, x, y)) {
                    worldPtr->tryMoveGhost(this, direction, dt);
                }
            }
            return;
        }

        // --- CHASE / FEAR mode ---
        Direction chosen = Direction::NONE;
        double snapX = std::floor(x) + 0.5;
        double snapY = std::floor(y) + 0.5;
        double snapEpsilon = 0.15;
        bool nearCenter = (std::abs(x - snapX) < snapEpsilon && std::abs(y - snapY) < snapEpsilon);

        // midden van gangpad
        double centerForceEpsilon = 0.20;
        if (direction == Direction::UP || direction == Direction::DOWN) {
            if (std::abs(x - snapX) < centerForceEpsilon) { x = snapX; setPosition(x, y); }
        } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
            if (std::abs(y - snapY) < centerForceEpsilon) { y = snapY; setPosition(x, y); }
        }

        // check ghostdoor
        bool nearDoor = false;
        for (const auto& door : worldPtr->getGhostDoors()) {
            if (door->isGhostInDoorZone(x, y)) { nearDoor = true; break; }
        }

        // Direction is omhoog (specifiek deze map)
        if (nearDoor && nearCenter) {
            direction = Direction::UP;
            desiredDirection = Direction::UP;
            decisionTimer = 0.0; // zorgen dat we niet direct terug beslissen
            worldPtr->tryMoveGhost(this, Direction::UP, dt);
            return;
        }

        bool canMove = worldPtr->canGhostMove(direction, x, y);
        bool atIntersection = nearCenter && worldPtr->isIntersection(snapX, snapY);

        // Gewoon rechtdoor gaan als ze niet bij een kruispunt zijn (als het kan)
        if (canMove && !atIntersection) {
            worldPtr->tryMoveGhost(this, direction, dt);
            return;
        }

        // Als ze geblokkeerd zijn, snap naar het midden
        if (!canMove) {
            if (direction == Direction::UP || direction == Direction::DOWN) {
                y = snapY;
            } else if (direction == Direction::LEFT || direction == Direction::RIGHT) {
                x = snapX;
            }
            setPosition(x, y);
            // Direct beslissing nemen omdat er is gesnapt
            decisionTimer = decisionCooldown;
        }

        // Cooldown voor beslissingen zodat ze niet blijven hangen
        if (atIntersection && decisionTimer < decisionCooldown) {
            worldPtr->tryMoveGhost(this, direction, dt);
            return;
        }

        // gebruik gecentreerde coords voor Free Directions wanneer we dichtbij tile center zijn
        double checkX = nearCenter ? snapX : x;
        double checkY = nearCenter ? snapY : y;

        std::vector<Direction> viableDirs = worldPtr->getFreeDirections(checkX, checkY);
        if (viableDirs.empty()) { return; }

        // --- LockedRandom ---
        if (type == GhostType::LockedRandom) {
            if (logic::Random::instance().nextDouble(0.0, 1.0) < 0.5) { // 50% kans op nieuwe richting
                Direction cand = viableDirs[
                    logic::Random::instance().nextInt(0, static_cast<int>(viableDirs.size()) - 1)
                ];
                if (isOpposite(cand, direction) && viableDirs.size() > 1) { // haal de 180 graden eruit als mogelijk
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

                worldPtr->tryMoveGhost(this, direction, dt);
            }
            return;
        }

        // --- Ahead/Direct chase ---
        auto pm = worldPtr->getPacman();
        double currentX = x, currentY = y;
        if (std::abs(x - snapX) < 0.15 && std::abs(y - snapY) < 0.15) { currentX = snapX; currentY = snapY; }

        std::vector<Direction> finalDirs = worldPtr->getFreeDirections(currentX, currentY);

        if (finalDirs.empty()) {
            return;
        }

        double refX = 0.0, refY = 0.0;
        bool maximize = (mode == Mode::Fear); // in fear mode wordt de manhatten afstand gemaximaliseert

        if (maximize) {
            // pacman is target (om zo ver mogelijk van weg te blijven)
            refX = pm->getX();
            refY = pm->getY();
        } else {
            if (type == GhostType::AheadOfPacman1 || type == GhostType::AheadOfPacman2) {
                std::tie(refX, refY) = worldPtr->predictStep(pm->getX(), pm->getY(), pm->getDirection()); // aheadOfPacman gebruikt tile voor pacman als target
            } else { // DirectChase gebruikt pacman zelf als target
                refX = pm->getX();
                refY = pm->getY();
            }
        }

        // Manhattan minimaliseren of maximaliseren afhankelijk van ghost mode
        double bestMetric = maximize ? -1e18 : 1e18;
        std::vector<Direction> bestDirs;
        for (Direction d : finalDirs) {
            auto [ghostNextX, ghostNextY] = worldPtr->predictStep(currentX, currentY, d);
            double metric = manhattan(ghostNextX, ghostNextY, refX, refY);
            if (maximize) {
                if (metric > bestMetric + 1e-9) { bestMetric = metric; bestDirs.clear(); bestDirs.push_back(d); }
                else if (std::abs(metric - bestMetric) < 1e-9) { bestDirs.push_back(d); }
            } else {
                if (metric < bestMetric - 1e-9) { bestMetric = metric; bestDirs.clear(); bestDirs.push_back(d); }
                else if (std::abs(metric - bestMetric) < 1e-9) { bestDirs.push_back(d); }
            }
        }

        // Kies een Direction (random als er meerdere keuzes zijn)
        if (!bestDirs.empty()) {
            Direction cand = bestDirs[logic::Random::instance().nextInt(0, static_cast<int>(bestDirs.size()) - 1)];

            // Liefst geen 180 graden draai
            if (!maximize && isOpposite(cand, direction) && bestDirs.size() > 1) {
                std::vector<Direction> nonOpp;
                for (Direction d : bestDirs) if (!isOpposite(d, direction)) nonOpp.push_back(d);
                if (!nonOpp.empty()) cand = nonOpp[logic::Random::instance().nextInt(0, static_cast<int>(nonOpp.size()) - 1)];
            }
            chosen = cand;
        }
        if (chosen != Direction::NONE) {
            desiredDirection = chosen;
            direction = chosen;
            decisionTimer = 0.0; // reset timer vanaf er een beslissing is genomen
            worldPtr->tryMoveGhost(this, direction, dt);
        } else {
            worldPtr->tryMoveGhost(this, direction, dt);
        }
    }
}