//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_WORLD_H
#define PACMAN_WORLD_H


#pragma once

#include <vector>
#include <memory>
#include "PacManModel.h"
#include "GhostModel.h"
#include "CoinModel.h"
#include "FruitModel.h"
#include "IEntityFactory.h"
#include "Score.h"

class World {
private:
    std::shared_ptr<IEntityFactory> factory;
    std::shared_ptr<PacManModel> pacman;
    std::vector<std::shared_ptr<GhostModel>> ghosts;
    std::vector<std::shared_ptr<CoinModel>> coins;
    std::vector<std::shared_ptr<FruitModel>> fruits;
    std::shared_ptr<Score> score;


    std::vector<std::vector<int>> maze; // 2D grid, 0 = empty, 1 = wall, 2 = coin, 3 = fruit
    int mazeHeight;
    int mazeWidth;

    void spawnEntitiesForLevel(int levelIndex);

public:
    World(std::shared_ptr<IEntityFactory> factory, std::shared_ptr<Score> score);
    void loadLevel(int levelIndex);
    void update(double dt);


    std::shared_ptr<PacManModel> getPacman() const { return pacman; }
    const std::vector<std::shared_ptr<GhostModel>>& getGhosts() const { return ghosts; }
    const std::vector<std::vector<int>>& getMaze() const { return maze; };
    std::vector<std::shared_ptr<CoinModel>> getCoins() const { return coins; }
    std::vector<std::shared_ptr<FruitModel>> getFruits() const { return fruits; }

    std::vector<Direction> getFreeDirections(double worldX, double worldY) const;
    std::pair<double,double> predictStep(double worldX, double worldY, Direction dir) const;


    bool isWallAt(double x, double y) const;
    void tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt);

};



#endif //PACMAN_WORLD_H