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
    [[nodiscard]] std::shared_ptr<PacManModel> getPacman() const { return pacman; }
    [[nodiscard]] const std::vector<std::shared_ptr<GhostModel>>& getGhosts() const { return ghosts; }
    [[nodiscard]] const std::vector<std::vector<int>>& getMaze() const { return maze; };
    [[nodiscard]] std::vector<std::shared_ptr<CoinModel>> getCoins() const { return coins; }
    [[nodiscard]] std::vector<std::shared_ptr<FruitModel>> getFruits() const { return fruits; }
    [[nodiscard]] std::vector<std::shared_ptr<GhostDoorModel>> getGhostDoors() const { return ghostDoors; }
    [[nodiscard]] std::shared_ptr<Score> getScore() const { return score; }

    // Getters Views
    [[nodiscard]] std::shared_ptr<EntityView> getPacmanView() const { return pacmanView; }
    [[nodiscard]] const std::vector<std::shared_ptr<EntityView>>& getGhostViews() const { return ghostViews; }
    [[nodiscard]] const std::vector<std::shared_ptr<EntityView>>& getCoinViews() const { return coinViews; }
    [[nodiscard]] const std::vector<std::shared_ptr<EntityView>>& getFruitViews() const { return fruitViews; }
    [[nodiscard]] const std::vector<std::shared_ptr<EntityView>>& getGhostDoorViews() const { return ghostDoorViews; }

    // Functies voor levels
    /**
     * @brief Spawnt de entities in de map. factory maakt views en observer worden toegevoegd.
     *
     * Entities worden aangemaakt en in de maze gezet. view en observer worden aangemaakt.
     * Ghosts types worden bepaalt op de spawn volgorde.
     */
    void spawnEntitiesForLevel();

    /**
     * @brief Gaat naar het volgende level (moeilijker).
     *
     * - Level gaat omhoog.
     * - Entities worden gereset naar hun startposities.
     * - Coins en Fruits worden gereset, dus worden ook terug zichtbaar.
     * - Moeilijkheid wordt aangepast door snelheid te verhogen en kortere fear mode.
     */
    void advanceLevel();

    /**
     * @brief Entities terug op hun spawn positie zetten.
     *
     * Overloopt alle entities en zet ze terug op hun spawn positie en startrichting.
     */
    void resetPositions();

    /**
     * @brief Start de death animatie van pacman.
     *
     * - Start de timer van de death animatie.
     * - Het bevriest alle ghosts.
     * - Stuurt naar de observer zodat de animatie kan beginnen.
     */
    void startDeathAnimatie();

    /**
     * @brief Update entities/level per tick
     *
     * Deze functie wordt elke tick aangeroepen door Levelstate.
     * - Het verwerkt input buffers, verplaatst pacman en update ghosts, coins en fruits.
     * - Het houdt de timeSinceLastcoin bij voor de score juist up te daten.
     * - Het kijkt of alle coins zijn verzameld en dus het level gedaan is en gaat naar het volgende level.
     * - Als pacman een ghost aanraakt in CHASE mode wordt de death animatie gestart.
     *
     * @param dt Delta time in seconden sinds de vorige update.
     */
    void update(double dt);

    // Collision Helpers

    /**
     * @brief Controleert of er een muur is.
     *
     * Zet wereldcoördinaten om naar gridcoördinaten en kijkt of de tile een muur is of buiten de map valt.
     *
     * @param x X-positie in wereldcoördinaten.
     * @param y Y-positie in wereldcoördinaten.
     * @return True als er een muur is (of buiten map), anders false.
     */
    bool isWallAt(double x, double y) const;

    /**
     * @brief Kijkt of een entity geblokkeerd wordt op een positie.
     *
     * Kijkt naar muren en eventueel ghost doors. Het gebruikt de radius van de entity.
     *
     * @param x X-positie in wereldcoördinaten.
     * @param y Y-positie in wereldcoördinaten.
     * @param radius Collision radius van de entity.
     * @param disallowDoor Indien true zijn ghost doors niet toegestaan.
     * @return True als de positie geblokkeerd is.
     */
    bool isBlockedAt(double x, double y, double radius, bool disallowDoor) const;

    /**
     * @brief Controleert of de tile een ghostdoor is.
     *
     * Zet wereldcoördinaten om naar gridcoördinaten en kijkt of de tile een ghostdoor is.
     *
     * @param x X-positie in wereldcoördinaten.
     * @param y Y-positie in wereldcoördinaten.
     * @return True als het een ghostdoor is, anders false.
     */
    bool isGhostDoor(double x, double y) const;

    // Grid helpers
    // hier verdergaan Marie
    bool isIntersection(double x, double y) const;
    /**
     * @brief Geeft alle mogelijke richtingen waar een entity naar kan bewegen.
     *
     * Berekent vanuit een wereldpositie welke richtingen geen muur bevatten.
     * Wordt gebruikt door PacMan en Ghost AI om geldige bewegingen te bepalen.
     *
     * @param worldX X-positie in wereldcoördinaten.
     * @param worldY Y-positie in wereldcoördinaten.
     * @return Vector met vrije richtingen.
     */
    std::vector<Direction> getFreeDirections(double worldX, double worldY) const;
    std::pair<double,double> predictStep(double worldX, double worldY, Direction dir) const;
    std::vector<Direction> findPath(int sx, int sy, int tx, int ty, bool allowDoor = false) const;

    // Movement Helpers
    void tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt);
    void tryMoveGhost(std::shared_ptr<Entity> e, Direction dir, double dt);
    bool canMoveIn(Direction dir, double x, double y) const;
    bool canGhostMove(Direction dir, double x, double y) const;
    bool isAlignedWithGrid(double x, double y) const;
};


#endif //PACMAN_WORLD_H