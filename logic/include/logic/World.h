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
#include "GhostDoorModel.h"
#include "IEntityFactory.h"
#include "Score.h"

/**
 * @class World
 * @brief Beheert alle game logica
 *
 * World is de controller.
 * Het creëert entities via de abstract factory en notificeert de observers via het
 * Observer‑pattern.
 * World is verantwoordelijk voor:
 *  - het spawnen en resetten van entities per level,
 *  - het updaten van game state per tick,
 *  - collision detection en pathfinding helpers voor pacman en de ghosts.
 *
 * @note World behoort tot de logic library en bevat geen SFML‑afhankelijkheden.
 */
class World {
private:
    std::shared_ptr<IEntityFactory> factory;
    std::shared_ptr<PacManModel> pacman;
    std::vector<std::shared_ptr<GhostModel>> ghosts;
    std::vector<std::shared_ptr<CoinModel>> coins;
    std::vector<std::shared_ptr<FruitModel>> fruits;
    std::vector<std::shared_ptr<GhostDoorModel>> ghostDoors;
    std::shared_ptr<Score> score;

    // Views
    std::shared_ptr<EntityView> pacmanView;
    std::vector<std::shared_ptr<EntityView>> ghostViews;
    std::vector<std::shared_ptr<EntityView>> coinViews;
    std::vector<std::shared_ptr<EntityView>> fruitViews;
    std::vector<std::shared_ptr<EntityView>> ghostDoorViews;


    std::vector<std::vector<int>> maze; // 2D grid, 0 = empty, 1 = wall, 2 = coin, 3 = fruit, 4 = pacman, 5 = ghost
    int mazeHeight;
    int mazeWidth;

    // Timer voor score en Level
    double timeSinceLastCoin = 0.0;
    int currentLevel = 1;

    // pacman death animatie
    bool deathAnimation = false;
    double deathTimer = 0.0;
    double deathDuration = 0.0;

public:
    World(std::shared_ptr<IEntityFactory> factory, std::shared_ptr<Score> score);

    // Getters Entities
    std::shared_ptr<PacManModel> getPacman() const { return pacman; }
    const std::vector<std::shared_ptr<GhostModel>>& getGhosts() const { return ghosts; }
    const std::vector<std::vector<int>>& getMaze() const { return maze; };
    std::vector<std::shared_ptr<CoinModel>> getCoins() const { return coins; }
    std::vector<std::shared_ptr<FruitModel>> getFruits() const { return fruits; }
    std::vector<std::shared_ptr<GhostDoorModel>> getGhostDoors() const { return ghostDoors; }
    std::shared_ptr<Score> getScore() const { return score; }

    // Getters Views
    std::shared_ptr<EntityView> getPacmanView() const { return pacmanView; }
    const std::vector<std::shared_ptr<EntityView>>& getGhostViews() const { return ghostViews; }
    const std::vector<std::shared_ptr<EntityView>>& getCoinViews() const { return coinViews; }
    const std::vector<std::shared_ptr<EntityView>>& getFruitViews() const { return fruitViews; }
    const std::vector<std::shared_ptr<EntityView>>& getGhostDoorViews() const { return ghostDoorViews; }

    // Functies voor levels
    void spawnEntitiesForLevel(int levelIndex);
    void loadLevel(int levelIndex);
    void advanceLevel();

    void resetPositions();
    void startDeathAnimatie();

    /**
     * @brief Update entities/level per tick
     *
     * Deze functie wordt elke tick aangeroepen door Levelstate.
     * - Het verwerkt input buffers, verplaatst pacman en update ghosts, coins en fruits.
     * - Het houdt de timeSinceLastcoin bij voor de score juist up te daten.
     * - Het kijkt of alle coins zijn verzameld en dus het level gedaan is en gaat naar het volgende level.
     * - Als pacman een ghost aanraakt in CHASE mode wordt de death animatie gestart.
     * -
     *
     * @param dt Delta time in seconden sinds de vorige update.
     */
    void update(double dt);

    // Direction helpers
    std::vector<Direction> getFreeDirections(double worldX, double worldY) const;
    std::pair<double,double> predictStep(double worldX, double worldY, Direction dir) const;
    std::vector<Direction> findPath(int sx, int sy, int tx, int ty, bool allowDoor = false) const;

    // Collision Helpers
    bool isWallAt(double x, double y) const;
    bool isBlockedAt(double x, double y, double radius, bool disallowDoor) const;
    bool isGhostDoor(double x, double y) const;
    bool isIntersection(double x, double y) const;

    // Movement Helpers
    void tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt);
    void tryMoveGhost(std::shared_ptr<Entity> e, Direction dir, double dt);
    bool canMoveIn(Direction dir, double x, double y) const;
    bool canGhostMove(Direction dir, double x, double y) const;
    bool isAlignedWithGrid(double x, double y) const;
};


#endif //PACMAN_WORLD_H