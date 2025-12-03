#include "logic/GhostModel.h"
#include "logic/World.h"
#include "logic/Random.h"
#include <cmath>

// Manhattan afstand helper
static double manhattan(double x1, double y1, double x2, double y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}


GhostModel::GhostModel() {
    // default spawn is 0,0 maar World roept setStartPosition aan
}

void GhostModel::setMode(Mode m) {
    mode = m;

    // Special behaviour when entering FEAR mode
    if (m == Mode::Fear) {
        speed = 0.5;               // ghosts are slower in fear mode
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
        speed = 1.2; // often ghosts move faster back to base
    }

    // When returning to chase mode, reset speed
    if (m == Mode::Chase) {
        speed = 0.8;
    }
}

GhostModel::Mode GhostModel::getMode() const {
    return mode;
}


Direction GhostModel::computeLockedDir(const World& world) {
    // possible directions (no walls)
    std::vector<Direction> dirs = world.getFreeDirections(x, y);

    if (dirs.empty()) {
        return locked; // trapped (should never happen)
    }

    // p = 0.5 random switch
    double r = logic::Random::instance().nextDouble(0.0, 1.0);
    if (r < 0.5) {
        // pick random viable direction
        int idx = logic::Random::instance().nextInt(0, dirs.size()-1);
        return dirs[idx];
    }

    // otherwise continue locked direction IF that direction is possible
    for (Direction d : dirs) {
        if (d == locked)
            return locked;
    }

    // locked is not viable, must pick a new random direction
    int idx = logic::Random::instance().nextInt(0, dirs.size()-1);
    return dirs[idx];
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



void GhostModel::update(double dt) {

    // MODE: WAITING
    if (mode == Mode::Waiting) {
        // do nothing (LevelState will switch to chase mode after N seconds)
        return;
    }

    // MODE: EATEN → return to spawn point
    if (mode == Mode::Eaten) {
        // Move directly toward startX,startY
        double dx = 0, dy = 0;
        if (std::abs(x - startX) > 0.01) {
            dx = (x < startX ? 1 : -1);
            locked = (dx > 0 ? Direction::RIGHT : Direction::LEFT);
        }
        if (std::abs(y - startY) > 0.01) {
            dy = (y < startY ? 1 : -1);
            locked = (dy > 0 ? Direction::DOWN : Direction::UP);
        }

        // normalize
        double len = std::abs(dx) + std::abs(dy);
        if (len > 0) {
            x += (dx / len) * speed * dt;
            y += (dy / len) * speed * dt;
        }

        // Arrived at spawn
        if (manhattan(x,y,startX,startY) < 0.05) {
            x = startX; y = startY;
            setMode(Mode::Chase);
        }
        return;
    }


    // ------------------------------------------
    // FOR CHASE / FEAR modes:
    // Movement depends on AI and World collisions.
    // World will restrict movement so we first ask World.
    // ------------------------------------------

    // Gather valid directions:
    // e.g. not opposite of current direction (optional)
    //      not going into wall
    std::vector<Direction> dirs =
        worldRef->getFreeDirections(x, y);     // <-- je moet worldRef toevoegen!

    if (dirs.empty()) {
        return; // trapped
    }

    Direction chosen = Direction::NONE;

    // MODE: CHASE
    if (mode == Mode::Chase) {
        // which ghost type? decided in LevelState, but here we just do standard chase
        // find direction that MINIMIZES Manhattan distance to Pac-Man

        double best = 99999.0;
        for (Direction d : dirs) {
            auto [nx, ny] = worldRef->predictStep(x, y, d); // world helper needed
            double dist = manhattan(nx, ny, worldRef->getPacman()->getX(),
                                            worldRef->getPacman()->getY());
            if (dist < best) {
                best = dist;
                chosen = d;
            }
        }
    }

    // MODE: FEAR
    if (mode == Mode::Fear) {
        // choose direction that MAXIMIZES distance to Pac-Man
        double best = -99999.0;
        for (Direction d : dirs) {
            auto [nx, ny] = worldRef->predictStep(x, y, d);
            double dist = manhattan(nx, ny, worldRef->getPacman()->getX(),
                                            worldRef->getPacman()->getY());
            if (dist > best) {
                best = dist;
                chosen = d;
            }
        }
    }

    // If somehow none chosen:
    if (chosen == Direction::NONE) {
        // fallback to random
        chosen = dirs[logic::Random::instance().nextInt(0, dirs.size()-1)];
    }

    locked = chosen;

    // Move in locked direction
    double dx = 0, dy = 0;
    switch (locked) {
        case Direction::UP:    dy = -1; break;
        case Direction::DOWN:  dy = 1; break;
        case Direction::LEFT:  dx = -1; break;
        case Direction::RIGHT: dx = 1; break;
        default: break;
    }

    double len = std::abs(dx) + std::abs(dy);
    if (len > 0) {
        x += (dx / len) * speed * dt;
        y += (dy / len) * speed * dt;
    }
}
