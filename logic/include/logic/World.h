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

    std::shared_ptr<EntityView> pacmanView;
    std::vector<std::shared_ptr<EntityView>> ghostViews;
    std::vector<std::shared_ptr<EntityView>> coinViews;
    std::vector<std::shared_ptr<EntityView>> fruitViews;


    std::vector<std::vector<int>> maze; // 2D grid, 0 = empty, 1 = wall, 2 = coin, 3 = fruit, 4 = pacman, 5 = ghost
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

    std::shared_ptr<EntityView> getPacmanView() const { return pacmanView; }
    const std::vector<std::shared_ptr<EntityView>>& getGhostViews() const { return ghostViews; }
    const std::vector<std::shared_ptr<EntityView>>& getCoinViews() const { return coinViews; }
    const std::vector<std::shared_ptr<EntityView>>& getFruitViews() const { return fruitViews; }

    std::vector<Direction> getFreeDirections(double worldX, double worldY) const;
    std::pair<double,double> predictStep(double worldX, double worldY, Direction dir) const;

    bool isWallAt(double x, double y) const;
    void tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt);
    bool canMoveIn(Direction dir, double x, double y) const;
    bool isAlignedWithGrid(double x, double y) const;

};



#endif //PACMAN_WORLD_H