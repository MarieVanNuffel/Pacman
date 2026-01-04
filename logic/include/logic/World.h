//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef pacman_WORLD_H
#define pacman_WORLD_H

#pragma once

#include <vector>
#include <memory>
#include "pacmanModel.h"
#include "GhostModel.h"
#include "CoinModel.h"
#include "FruitModel.h"
#include "GhostDoorModel.h"
#include "IEntityFactory.h"
#include "Score.h"

namespace logic {
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
    class World : public std::enable_shared_from_this<World> {
    private:
        std::shared_ptr<IEntityFactory> factory;
        std::shared_ptr<PacManModel> pacman;
        std::vector<std::shared_ptr<GhostModel>> ghosts;
        std::vector<std::shared_ptr<CoinModel>> coins;
        std::vector<std::shared_ptr<FruitModel>> fruits;
        std::vector<std::shared_ptr<GhostDoorModel>> ghostDoors;
        std::shared_ptr<Score> score;

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

        /**
         * @brief Geeft het pacman model terug.
         * @return Shared pointer naar pacmanModel.
         */
        [[nodiscard]] std::shared_ptr<PacManModel> getPacman() const { return pacman; }
        /**
         * @brief Geeft alle ghosts in de wereld terug.
         * @return Const referentie naar vector met GhostModel pointers.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<GhostModel>>& getGhosts() const { return ghosts; }
        /**
         * @brief Geeft de maze grid terug.
         * @return Const referentie naar de 2D maze vector.
         */
        [[nodiscard]] const std::vector<std::vector<int>>& getMaze() const { return maze; };
        /**
         * @brief Geeft alle coins terug.
         * @return Vector met CoinModel pointers.
         */
        [[nodiscard]] std::vector<std::shared_ptr<CoinModel>> getCoins() const { return coins; }
        /**
         * @brief Geeft alle fruits terug.
         * @return Vector met FruitModel pointers.
         */
        [[nodiscard]] std::vector<std::shared_ptr<FruitModel>> getFruits() const { return fruits; }
        /**
         * @brief Geeft alle ghost doors terug.
         * @return Vector met GhostDoorModel pointers.
         */
        [[nodiscard]] std::vector<std::shared_ptr<GhostDoorModel>> getGhostDoors() const { return ghostDoors; }
        /**
         * @brief Geeft het score object terug.
         * @return Shared pointer naar Score.
         */
        [[nodiscard]] std::shared_ptr<Score> getScore() const { return score; }


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

        /**
         * @brief Kijkt of we dicht bij het midden van een tile zitten
         *
         * Kijkt met een kleine marge of we al dichtbij een tile center zitten om uiteindelijk te zien of we van richting mogen veranderen.
         *
         * @param x X-positie in wereldcoördinaten.
         * @param y Y-positie in wereldcoördinaten.
         * @return True als we uitgelijnd zijn met de grid, anders false
         */
        bool isAlignedWithGrid(double x, double y) const;

        /**
         * @brief Kijkt of we op een kruispunt zitten
         *
         * Kijkt of er 3 of meer vrije richtingen zijn vanaf een positie (en dus een kruispunt).
         *
         * @param x X-positie in wereldcoördinaten.
         * @param y Y-positie in wereldcoördinaten.
         * @return True als de positie een kruispunt is.
         */
        bool isIntersection(double x, double y) const;

        /**
         * @brief Geeft alle mogelijke richtingen waar een entity naar kan bewegen.
         *
         * Berekent vanuit een wereldpositie welke richtingen geen muur bevatten.
         *
         * @param worldX X-positie in wereldcoördinaten.
         * @param worldY Y-positie in wereldcoördinaten.
         * @return Vector met beschikbare richtingen.
         */
        std::vector<Direction> getFreeDirections(double worldX, double worldY) const;

        /**
         * @brief Voorspelt de volgende stap in een bepaalde richting.
         *
         * Berekent de volgende wereldpositie wanneer een entity één stap zou zetten in de richting dat die nu is aan het gaan.
         *
         * @param worldX Start X-positie.
         * @param worldY Start Y-positie.
         * @param dir Richting waarin bewogen wordt.
         * @return De voorspelde positie.
         */
        std::pair<double,double> predictStep(double worldX, double worldY, Direction dir) const;


        // Movement Helpers

        /**
        * @brief Probeert een entity te verplaatsen in een richting.
        *
        * Controleert collisions en verplaatst de entity enkel als het mag.
        *
        * @param e Entity die bewogen wordt.
        * @param dir Richting waarin bewogen wordt.
        * @param dt Delta time sinds de vorige update.
        */
        void tryMoveEntity(std::shared_ptr<Entity> e, Direction dir, double dt);

        /**
        * @brief Probeert een ghost te verplaatsen in een richting.
        *
        * Gelijkaardig aan tryMoveEntity, maar met ghost-specifieke regels zoals ghost doors en movement states.
        *
        * @param e Ghost entity.
        * @param dir Richting waarin bewogen wordt.
        * @param dt Delta time sinds de vorige update.
     */
        void tryMoveGhost(Entity* e, Direction dir, double dt);

        /**
        * @brief Controleert of een entity kan bewegen in een richting.
        *
        * Houdt rekening met muren en ghostdoors.
        *
        * @param dir Richting om te testen.
        * @param x X-positie in wereldcoördinaten.
        * @param y Y-positie in wereldcoördinaten.
        * @return True als beweging mogelijk is.
        */
        bool canMoveIn(Direction dir, double x, double y) const;

        /**
         * @brief Controleert of een ghost kan bewegen in een richting.
         *
         * Versie van canMoveIn met extra ghost-regels zoals ghostdoors.
         *
         * @param dir Richting om te testen.
         * @param x X-positie in wereldcoördinaten.
         * @param y Y-positie in wereldcoördinaten.
         * @return True als de ghost mag bewegen.
         */
        bool canGhostMove(Direction dir, double x, double y) const;

        // Pathfinding voor breadth first
        /**
         * @brief Zoekt een pad tussen twee gridposities.
         *
         * Gebruikt breadth first om een richtingen te bepalen van startpositie (sx,sy) naar target (tx,ty).
         *
         * Wordt voornamelijk gebruikt om ghosts terug naar spawn te laten gaan wanneer ze opgegeten worden.
         *
         * @param sx Start X-coördinaat (grid).
         * @param sy Start Y-coördinaat (grid).
         * @param tx Target X-coördinaat (grid).
         * @param ty Target Y-coördinaat (grid).
         * @param allowDoor Of ghost doors mogen worden gebruikt.
         * @return Vector van richtingen die het pad vormen.
         */
        std::vector<Direction> findPath(int sx, int sy, int tx, int ty, bool allowDoor = false) const;
    };
}

#endif //pacman_WORLD_H