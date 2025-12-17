//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/World.h"

#include <iostream>

#include "logic/Random.h"
#include <stdexcept>


World::World(std::shared_ptr<IEntityFactory> factory_, std::shared_ptr<Score> score_)
    : factory(factory_), score(score_)
{
    maze = {
        {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,3,2,2,2,1,2,2,2,2,2,2,2,2,1,2,2,2,3,1},
            {1,2,1,1,2,1,2,1,1,1,1,1,1,2,1,2,1,1,2,1},
            {1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},
            {1,2,1,2,1,1,2,1,1,2,2,1,1,2,1,1,2,1,2,1},
            {1,2,2,2,2,2,2,1,5,5,5,5,1,2,2,2,2,2,2,1},
            {1,2,1,2,1,1,2,1,1,1,1,1,1,2,1,1,2,1,2,1},
            {1,2,1,2,2,2,2,2,2,4,2,2,2,2,2,2,2,1,2,1},
            {1,2,1,1,2,1,2,1,1,1,1,1,1,2,1,2,1,1,2,1},
            {1,3,2,2,2,1,2,2,2,2,2,2,2,2,1,2,2,2,3,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        }


    };


    mazeHeight = maze.size();
    mazeWidth = maze[0].size();

    loadLevel(1);
}


void World::spawnEntitiesForLevel(int levelIndex) {
    // --- PAC-MAN ---
    pacman = std::make_shared<PacManModel>();
    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 4) {
                pacman->setPosition(x + 0.5, y + 0.5);
            }
        }
    }
    pacmanView = factory->createPacmanView(pacman.get());

    // --- GHOSTS ---
    ghosts.clear();
    ghostViews.clear();

    int ghostIndex = 0;

    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 5) {
                // Bepaal type ghost op basis van spawn volgorde
                GhostModel::GhostType type;
                switch (ghostIndex) {
                    case 0: type = GhostModel::GhostType::LockedRandom; break;
                    case 1: type = GhostModel::GhostType::AheadOfPacman1; break;
                    case 2: type = GhostModel::GhostType::AheadOfPacman2; break;
                    case 3: type = GhostModel::GhostType::DirectChase; break;
                }

                auto ghost = std::make_shared<GhostModel>(type);
                ghost->setStartPosition(x + 0.5, y + 0.5);
                ghost->setWorld(this);  // super belangrijk voor AI

                ghosts.push_back(ghost);
                ghostViews.push_back(factory->createGhostView(ghost.get()));

                ghostIndex++;
            }
        }
    }

    // --- COINS ---
    coins.clear();
    coinViews.clear();
    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 2) {
                auto c = std::make_shared<CoinModel>();
                c->setPosition(x + 0.5, y + 0.5);
                coins.push_back(c);
                coinViews.push_back(factory->createCoinView(c.get()));
                c->addObserver(score.get());
            }
        }
    }

    // --- FRUITS ---
    fruits.clear();
    fruitViews.clear();
    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 3) {
                auto f = std::make_shared<FruitModel>();
                f->setPosition(x + 0.5, y + 0.5);
                fruits.push_back(f);
                fruitViews.push_back(factory->createFruitView(f.get()));
                f->addObserver(score.get());
            }
        }
    }
}





void World::loadLevel(int levelIndex) {
    // TODO: change maze per level or randomize
    spawnEntitiesForLevel(levelIndex);
}


#include "logic/World.h"
#include <cmath>

void World::update(double dt) {
    auto pm = pacman;
    if (!pm) return;

    double gridCenterX = std::floor(pm->getX()) + 0.5;
    double gridCenterY = std::floor(pm->getY()) + 0.5;
    double snapEpsilon = 0.1;

    // --- 1) Buffered input: probeer DesiredDirection ---
    if (pm->getDesiredDirection() != Direction::NONE &&
        canMoveIn(pm->getDesiredDirection(), pm->getX(), pm->getY())) {
        pm->setDirection(pm->getDesiredDirection());
    }

    // --- 2) Corner snapping (alleen bij tile center) ---
    if (pm->getDirection() == Direction::UP || pm->getDirection() == Direction::DOWN) {
        if (std::abs(pm->getX() - gridCenterX) < snapEpsilon)
            pm->setPosition(gridCenterX, pm->getY());
    } else if (pm->getDirection() == Direction::LEFT || pm->getDirection() == Direction::RIGHT) {
        if (std::abs(pm->getY() - gridCenterY) < snapEpsilon)
            pm->setPosition(pm->getX(), gridCenterY);
    }

    // --- 3) Probeer huidige richting te bewegen, anders gewenste richting ---
    if (canMoveIn(pm->getDirection(), pm->getX(), pm->getY())) {
        tryMoveEntity(pm, pm->getDirection(), dt);
    } else {
        pm->setDirection(Direction::NONE);
    }

    // --- 4) Update ghosts ---
    for (auto& ghost : ghosts) {
        ghost->update(dt);
    }
}

void World::tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt) {
    if (!e || dir == Direction::NONE) return;

    double step = e->getSpeed() * dt;
    double nx = e->getX();
    double ny = e->getY();

    switch (dir) {
        case Direction::UP:    ny -= step; break;
        case Direction::DOWN:  ny += step; break;
        case Direction::LEFT:  nx -= step; break;
        case Direction::RIGHT: nx += step; break;
        default: break;
    }

    double radius = 0.45; // iets kleiner dan 0.5 zodat Pac-Man niet vastzit

    // check collision alle vier hoeken
    if (isWallAt(nx - radius, ny - radius) ||
        isWallAt(nx - radius, ny + radius) ||
        isWallAt(nx + radius, ny - radius) ||
        isWallAt(nx + radius, ny + radius)) {
        return;
    }

    e->setPosition(nx, ny);
}

bool World::canMoveIn(Direction dir, double x, double y) const {
    if (dir == Direction::NONE) return false;

    double checkStep = 0.05; // kleine check-step
    double nx = x;
    double ny = y;

    switch(dir) {
        case Direction::UP:    ny -= checkStep; break;
        case Direction::DOWN:  ny += checkStep; break;
        case Direction::LEFT:  nx -= checkStep; break;
        case Direction::RIGHT: nx += checkStep; break;
        default: break;
    }

    double radius = 0.49;

    return !(isWallAt(nx - radius, ny - radius) ||
             isWallAt(nx - radius, ny + radius) ||
             isWallAt(nx + radius, ny - radius) ||
             isWallAt(nx + radius, ny + radius));
}




std::vector<Direction> World::getFreeDirections(double x, double y) const {
    std::vector<Direction> dirs;

    if (!isWallAt(x, y - 0.01)) dirs.push_back(Direction::UP);
    if (!isWallAt(x, y + 0.01)) dirs.push_back(Direction::DOWN);
    if (!isWallAt(x - 0.01, y)) dirs.push_back(Direction::LEFT);
    if (!isWallAt(x + 0.01, y)) dirs.push_back(Direction::RIGHT);

    return dirs;
}



// Predict next position after moving one step in direction d
std::pair<double, double> World::predictStep(double x, double y, Direction d) const {
    double nx = x, ny = y;
    switch (d) {
        case Direction::UP:    ny -= 0.05; break;
        case Direction::DOWN:  ny += 0.05; break;
        case Direction::LEFT:  nx -= 0.05; break;
        case Direction::RIGHT: nx += 0.05; break;
        default: break;
    }
    return {nx, ny};
}

// Helper for getFreeDirections to check collision at float position
bool World::isWallAt(double x, double y) const {
    int cx = static_cast<int>(x);
    int cy = static_cast<int>(y);

    if (cx < 0 || cy < 0 || cx >= mazeWidth || cy >= mazeHeight)
        return true; // buiten de map = muur

    return maze[cy][cx] == 1;
}


bool World::isAlignedWithGrid(double x, double y) const {
    int gridX = static_cast<int>(x);
    int gridY = static_cast<int>(y);

    // epsilon bepaalt hoeveel afwijking toegestaan is
    const double epsilon = 0.05;
    return std::abs(x - (gridX + 0.5)) < epsilon &&
           std::abs(y - (gridY + 0.5)) < epsilon;
}
