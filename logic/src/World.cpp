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
        {1,1,1,1,1,1,1},
        {1,2,2,2,2,2,1},
        {1,2,1,1,1,2,1},
        {1,2,2,2,2,2,1},
        {1,1,1,1,1,1,1}
    };
    mazeHeight = maze.size();
    mazeWidth = maze[0].size();

    loadLevel(1);
}


void World::spawnEntitiesForLevel(int levelIndex) {
    pacman = std::make_shared<PacManModel>();
    // place pacman at center-ish
    pacman->setPosition(0.0, 0.6);
    ghosts.clear();
    for(int i=0;i<4;++i) {
        auto g = std::make_shared<GhostModel>();
        g->setStartPosition(0.0, 0.0);
        ghosts.push_back(g);
    }
    coins.clear(); fruits.clear();
    // spawn coins for every cell with 2
    for(size_t y=0;y<maze.size();++y) {
        for(size_t x=0;x<maze[y].size();++x) {
            if(maze[y][x]==2) {
                auto c = std::make_shared<CoinModel>();
                // convert cell to normalized coords
                double nx = (double)x / (double)maze[y].size() * 2.0 - 1.0 +
                1.0/maze[y].size();
                double ny = (double)y / (double)maze.size() * 2.0 - 1.0 +
                1.0/maze.size();
                c->setPosition(nx, ny);
                coins.push_back(c);
            }
        }
    }
}

void World::loadLevel(int levelIndex) {
    // TODO: change maze per level or randomize
    spawnEntitiesForLevel(levelIndex);
}

void World::update(double dt) {
    // update pacman and ghosts
    pacman->update(dt);
    for(auto &g: ghosts) g->update(dt);
    // collision checks: pacman with coins / fruits / ghosts
}


void World::tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double
dt) {
    // minimal sample: change entity position according to dir, ignoring walls.
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
    int cx = static_cast<int>((x + 1.0) / 2.0 * mazeWidth);
    int cy = static_cast<int>((y + 1.0) / 2.0 * mazeHeight);
    return isWallAt(cx, cy);
}