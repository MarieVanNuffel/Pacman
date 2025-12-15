//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/World.h"
#include "logic/Random.h"
#include <stdexcept>


World::World(std::shared_ptr<IEntityFactory> factory_, std::shared_ptr<Score> score_)
    : factory(factory_), score(score_)
{
    maze = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,3,2,2,2,1,2,2,2,2,2,1,2,3,1},
    {1,2,1,1,2,1,2,1,1,1,2,1,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
    {1,2,2,2,2,2,2,5,5,2,2,2,2,2,1},
    {1,1,1,1,2,1,1,5,5,1,1,2,1,1,1},
    {1,2,2,2,2,2,2,5,5,2,2,2,2,2,1},
    {1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,2,1,2,1,1,1,2,1,2,2,1},
    {1,2,2,2,2,1,2,2,2,2,2,1,2,2,1},
    {1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
    {1,4,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
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
            if (maze[y][x] == 4) pacman->setPosition(x + 0.5, y + 0.5);
        }
    }
    pacmanView = factory->createPacmanView(pacman.get());

    // --- GHOSTS ---
    ghosts.clear();
    ghostViews.clear();
    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 5) {
                auto g = std::make_shared<GhostModel>();
                g->setStartPosition(x + 0.5, y + 0.5);
                ghosts.push_back(g);
                ghostViews.push_back(factory->createGhostView(g.get()));
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

                // Score observer koppelen
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

                // Score observer koppelen
                f->addObserver(score.get());
            }
        }
    }
}



void World::loadLevel(int levelIndex) {
    // TODO: change maze per level or randomize
    spawnEntitiesForLevel(levelIndex);
}


void World::update(double dt) {
    auto pm = pacman;

    // 1) Alleen in het midden van een tile mag je draaien
    if (isAlignedWithGrid(pm->getX(), pm->getY())) {

        // Probeer gewenste richting
        if (canMoveIn(pm->getDesiredDirection(), pm->getX(), pm->getY())) {
            pm->setDirection(pm->getDesiredDirection());
        }
    }

    // 2) Als huidige richting niet meer kan → stoppen
    if (!canMoveIn(pm->getDirection(), pm->getX(), pm->getY())) {
        pm->setDirection(Direction::NONE);
    }

    // 3) Beweeg in huidige richting
    tryMoveEntity(pm, pm->getDirection(), dt);
}







void World::tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt) {
    if (!e || dir == Direction::NONE) return;

    double step = e->getSpeed() * dt;
    double nx = e->getX();
    double ny = e->getY();

    // Probeer nieuwe positie
    switch (dir) {
        case Direction::UP:    ny -= step; break;
        case Direction::DOWN:  ny += step; break;
        case Direction::LEFT:  nx -= step; break;
        case Direction::RIGHT: nx += step; break;
        default: break;
    }

    // radius van Pac-Man (half van sprite in grid units)
    double radius = 0.45; // iets kleiner dan 0.5 zodat hij niet vastzit
    bool canMove = true;

    // check alle vier de hoeken
    if (isWallAt(nx - radius, ny - radius) ||
        isWallAt(nx - radius, ny + radius) ||
        isWallAt(nx + radius, ny - radius) ||
        isWallAt(nx + radius, ny + radius)) {
        canMove = false;
        }

    if (canMove) {
        e->setPosition(nx, ny);
    }
}


// Return a list of possible directions the entity can move without hitting a wall
std::vector<Direction> World::getFreeDirections(double x, double y) const {
    std::vector<Direction> dirs;

    if (!isWallAt(x, y + 0.01)) dirs.push_back(Direction::UP);
    if (!isWallAt(x, y - 0.01)) dirs.push_back(Direction::DOWN);
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

bool World::canMoveIn(Direction dir, double x, double y) const {
    if (dir == Direction::NONE) return false;

    double step = 0.01; // kleine stap voor check
    double nx = x, ny = y;

    switch(dir) {
        case Direction::UP:    ny -= step; break;
        case Direction::DOWN:  ny += step; break;
        case Direction::LEFT:  nx -= step; break;
        case Direction::RIGHT: nx += step; break;
        default: break;
    }

    double radius = 0.45;
    return !(isWallAt(nx - radius, ny - radius) ||
             isWallAt(nx - radius, ny + radius) ||
             isWallAt(nx + radius, ny - radius) ||
             isWallAt(nx + radius, ny + radius));
}

bool World::isAlignedWithGrid(double x, double y) const {
    int gridX = static_cast<int>(x);
    int gridY = static_cast<int>(y);

    // epsilon bepaalt hoeveel afwijking toegestaan is
    const double epsilon = 0.01;
    return std::abs(x - (gridX + 0.5)) < epsilon &&
           std::abs(y - (gridY + 0.5)) < epsilon;
}
