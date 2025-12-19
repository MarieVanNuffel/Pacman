#include "logic/GhostModel.h"
#include "logic/World.h"
#include "logic/Random.h"
#include <cmath>
#include <iostream>
#include <ostream>

// Manhattan afstand helper
static double manhattan(double x1, double y1, double x2, double y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}


GhostModel::GhostModel(GhostType t) : type(t) {
    speed = 2.8;
    locked = Direction::NONE;     // 🔥 ALTIJD start omhoog
    direction = Direction::UP;  // voor animatie
    mode = Mode::Waiting;
}



void GhostModel::setMode(Mode m) {
    mode = m;

    // Special behaviour when entering FEAR mode
    if (m == Mode::Fear) {
        speed = 1.5;               // ghosts are slower in fear mode
        // reverse direction
        switch (locked) {
            case Direction::UP:    locked = Direction::DOWN; break;
            case Direction::DOWN:  locked = Direction::UP; break;
            case Direction::LEFT:  locked = Direction::RIGHT; break;
            case Direction::RIGHT: locked = Direction::LEFT; break;
            default: break;
        }
    }

    // When eaten, slow down even more (optional)
    if (m == Mode::Eaten) {
        speed = 3.0; // often ghosts move faster back to base
    }

    // When returning to chase mode, reset speed
    if (m == Mode::Chase) {
        speed = 2.8;
    }
}

GhostModel::Mode GhostModel::getMode() const {
    return mode;
}


Direction GhostModel::computeLockedDir(const World& world)
{
    auto dirs = world.getFreeDirections(x, y);
    if (dirs.empty()) return locked;

    // 50% kans om random te switchen
    if (logic::Random::instance().nextDouble(0.0, 1.0) < 0.5) {
        return dirs[
            logic::Random::instance().nextInt(0, dirs.size() - 1)
        ];
    }

    // anders: behoud locked als mogelijk
    for (Direction d : dirs) {
        if (d == locked)
            return locked;
    }

    // locked niet mogelijk → random
    return dirs[
        logic::Random::instance().nextInt(0, dirs.size() - 1)
    ];
}



/// DECIDE DIRECTION (generic fallback)
// Will never be directly called by the game logic.
// Specific behaviors (locked, chasing, fear) are handled separately.
Direction GhostModel::decideDirection() {
    int r = logic::Random::instance().nextInt(0, 3);
    switch (r) {
        case 0: return Direction::UP;
        case 1: return Direction::DOWN;
        case 2: return Direction::LEFT;
        default: return Direction::RIGHT;
    }
}



void GhostModel::update(double dt)
{
    if (!worldRef) return;

    releaseTimer += dt;

    // ------------------------------------------------
    // 1) WAITING → CHASE (release timing)
    // ------------------------------------------------
    // ---------------------------------
    // WAITING: simpel naar boven bewegen
    // ---------------------------------
    if (mode == Mode::Waiting) {

        // release timing
        if (type == GhostType::LockedRandom ||
            type == GhostType::AheadOfPacman1 ||
            (type == GhostType::AheadOfPacman2 && releaseTimer >= 5.0) ||
            (type == GhostType::DirectChase && releaseTimer >= 10.0)) {

            mode = Mode::Chase;
            }

        // beweeg altijd omhoog uit de box
        locked = Direction::UP;
        direction = Direction::UP;

        worldRef->tryMoveEntity(
            std::shared_ptr<Entity>(this, [](Entity*){}),
            Direction::UP,
            dt
        );
        return;
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
            mode = Mode::Chase;
        }
        return;
    }

    // ------------------------------------------------
    // 3) Als niet op kruispunt → blijf rechtdoor
    // ------------------------------------------------
    if (!worldRef->isIntersection(x, y)) {
        direction = locked;
        worldRef->tryMoveEntity(
            std::shared_ptr<Entity>(this, [](Entity*){}),
            locked,
            dt
        );
        return;
    }

    // ------------------------------------------------
    // 4) Richting kiezen op kruispunt
    // ------------------------------------------------
    auto dirs = worldRef->getFreeDirections(x, y);
    if (dirs.empty()) return;

    Direction chosen = locked;
    auto pm = worldRef->getPacman();

    // ------------------------------------------------
    // Ghost 1: locked / random
    // ------------------------------------------------
    if (type == GhostType::LockedRandom) {
        chosen = computeLockedDir(*worldRef);
    }

    // ------------------------------------------------
    // Ghost 2 & 3: vóór Pac-Man
    // ------------------------------------------------
    else if (type == GhostType::AheadOfPacman1 ||
             type == GhostType::AheadOfPacman2) {

        auto [tx, ty] = worldRef->predictStep(
            pm->getX(), pm->getY(), pm->getDirection()
        );

        double best = 1e9;
        std::vector<Direction> bestDirs;

        for (Direction d : dirs) {
            auto [nx, ny] = worldRef->predictStep(x, y, d);
            double dist = std::abs(nx - tx) + std::abs(ny - ty);

            if (dist < best - 0.0001) {
                best = dist;
                bestDirs.clear();
                bestDirs.push_back(d);
            }
            else if (std::abs(dist - best) < 0.0001) {
                bestDirs.push_back(d);
            }
        }

        chosen = bestDirs[
            logic::Random::instance().nextInt(0, bestDirs.size() - 1)
        ];
    }

    // ------------------------------------------------
    // Ghost 4: directe chase
    // ------------------------------------------------
    else if (type == GhostType::DirectChase) {

        double best = 1e9;
        std::vector<Direction> bestDirs;

        for (Direction d : dirs) {
            auto [nx, ny] = worldRef->predictStep(x, y, d);
            double dist = std::abs(nx - pm->getX()) +
                          std::abs(ny - pm->getY());

            if (dist < best - 0.0001) {
                best = dist;
                bestDirs.clear();
                bestDirs.push_back(d);
            }
            else if (std::abs(dist - best) < 0.0001) {
                bestDirs.push_back(d);
            }
        }

        chosen = bestDirs[
            logic::Random::instance().nextInt(0, bestDirs.size() - 1)
        ];
    }

    // ------------------------------------------------
    // 5) Bewegen
    // ------------------------------------------------
    locked = chosen;
    direction = locked;
    worldRef->tryMoveEntity(
        std::shared_ptr<Entity>(this, [](Entity*){}),
        locked,
        dt
    );
}

