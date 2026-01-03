//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/World.h"

#include <iostream>
#include <queue>
#include "logic/Random.h"
#include <stdexcept>


World::World(std::shared_ptr<IEntityFactory> factory_, std::shared_ptr<Score> score_)
    : factory(factory_), score(score_)
{
    maze = {
        {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,3,2,2,2,1,2,2,2,2,2,2,2,2,1,2,2,2,2,1},
            {1,2,1,1,2,1,2,1,1,1,1,1,1,2,1,2,1,1,2,1},
            {1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},
            {1,2,1,2,1,1,2,1,1,6,6,1,1,2,1,1,2,1,2,1},
            {1,2,2,2,2,2,2,1,5,5,5,5,1,2,2,2,2,2,2,1},
            {1,2,1,2,1,1,2,1,1,1,1,1,1,2,1,1,2,1,2,1},
            {1,2,1,2,2,2,2,2,2,4,2,2,2,2,2,2,2,1,2,1},
            {1,2,1,1,2,1,2,1,1,1,1,1,1,2,1,2,1,1,2,1},
            {1,2,2,2,2,1,2,2,2,2,2,2,2,2,1,2,2,2,3,1},
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
    {
        // factory returns {view, optional observer}
        auto pv = factory->createPacmanView(pacman.get());
        pacmanView = pv.first;
        auto pacmanObs = pv.second;
        // register observer on pacman model if view provided an observer
        if (pacmanObs) {
            pacman->addObserver(pacmanObs); // only if PacManModel inherits Subject
        }
    }

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
                    case 0: type = GhostModel::GhostType::AheadOfPacman1; break;
                    case 1: type = GhostModel::GhostType::LockedRandom; break;
                    case 2: type = GhostModel::GhostType::AheadOfPacman2; break;
                    case 3: type = GhostModel::GhostType::DirectChase; break;
                    default: type = GhostModel::GhostType::LockedRandom; break;
                }

                auto ghost = std::make_shared<GhostModel>(type);
                ghost->setStartPosition(x + 0.5, y + 0.5);
                ghost->setWorld(this);  // super belangrijk voor AI

                ghosts.push_back(ghost);

                // factory returns pair: {view, optional observer}
                auto gv = factory->createGhostView(ghost.get());
                ghostViews.push_back(gv.first);
                if (gv.second) {
                    ghost->addObserver(gv.second);
                }

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

                // De factory retourneert: {shared_ptr<EntityView>, shared_ptr<Observer>}
                auto viewPair = factory->createCoinView(c.get());
                auto cv = viewPair.first;
                auto cvObs = viewPair.second;

                // Bewaar de view zodat het object in leven blijft
                coinViews.push_back(cv);

                // Registreer Score als observer (Score : public Observer)
                c->addObserver(std::static_pointer_cast<Observer>(score));

                // Als de concrete view zelf een Observer was, registreer die ook
                if (cvObs) {
                    c->addObserver(cvObs);
                }
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

                // factory returns pair: {shared_ptr<EntityView>, shared_ptr<Observer>}
                auto viewPair = factory->createFruitView(f.get());
                auto fv = viewPair.first;
                auto fvObs = viewPair.second;

                // bewaar view zodat deze in leven blijft
                fruitViews.push_back(fv);

                // registreer Score als observer
                f->addObserver(std::static_pointer_cast<Observer>(score));

                // registreer view als observer indien concrete view dat aanbiedt
                if (fvObs) {
                    f->addObserver(fvObs);
                }
            }
        }
    }

    // --- GHOSTDOOR ---
    ghostDoors.clear();
    ghostDoorViews.clear();

    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 6) {
                auto door = std::make_shared<GhostDoorModel>();
                door->setPosition(x + 0.5, y + 0.5);
                ghostDoors.push_back(door);

                auto dv = factory->createGhostDoorView(door.get());
                ghostDoorViews.push_back(dv.first);
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

    if (deathAnimation) {
        deathTimer += dt;
        if (deathTimer >= deathDuration) {
            pacman->loseLife(); // - leven pacman
            resetPositions(); // terug naar startposities

            pacman->revive(); // notify de observers

            for (auto& g : ghosts)
                g->setFrozen(false); // zorg dat de ghosts terug bewegen

            deathAnimation = false;
            deathTimer = 0.0;
        }
        // Skip de rest van de update functie
        return;
    }

    // update timer voor coin timing
    timeSinceLastCoin += dt;

    double gridCenterX = std::floor(pm->getX()) + 0.5;
    double gridCenterY = std::floor(pm->getY()) + 0.5;
    double snapEpsilon = 0.1;

    // --- PACMAN MOVEMENT ---
    // 1) probeer DesiredDirection
    // probeer direct als het mogelijk is vanaf huidige positie
    if (pm->getDesiredDirection() != Direction::NONE) {
        if (canMoveIn(pm->getDesiredDirection(), pm->getX(), pm->getY())) {
            pm->setDirection(pm->getDesiredDirection());
        } else {
            bool nearCenter = (std::abs(pm->getX() - gridCenterX) < snapEpsilon &&
                               std::abs(pm->getY() - gridCenterY) < snapEpsilon);
            if (nearCenter && canMoveIn(pm->getDesiredDirection(), gridCenterX, gridCenterY)) { // is het mogelijk vanaf tile center
                // snap naar center en zet direction naar DesiredDirection
                pm->setPosition(gridCenterX, gridCenterY);
                pm->setDirection(pm->getDesiredDirection());
            }
        }
    }

    // 2) Corner snapping bij tile centers
    if (pm->getDirection() == Direction::UP || pm->getDirection() == Direction::DOWN) {
        if (std::abs(pm->getX() - gridCenterX) < snapEpsilon)
            pm->setPosition(gridCenterX, pm->getY());
    } else if (pm->getDirection() == Direction::LEFT || pm->getDirection() == Direction::RIGHT) {
        if (std::abs(pm->getY() - gridCenterY) < snapEpsilon)
            pm->setPosition(pm->getX(), gridCenterY);
    }

    // 3) Probeer huidige richting te bewegen, anders DesiredDirection
    if (canMoveIn(pm->getDirection(), pm->getX(), pm->getY())) {
        tryMoveEntity(pm, pm->getDirection(), dt);
    } else {
        // Als de huidige richting is geblokkeerd, dan sta je stil
        pm->setDirection(Direction::NONE);

        // Als we dicht bij de tile center zijn, probeer desiredDirection
        if (pm->getDesiredDirection() != Direction::NONE) {
            bool nearCenter = (std::abs(pm->getX() - gridCenterX) < snapEpsilon &&
                               std::abs(pm->getY() - gridCenterY) < snapEpsilon);
            if (nearCenter && canMoveIn(pm->getDesiredDirection(), gridCenterX, gridCenterY)) {
                pm->setPosition(gridCenterX, gridCenterY);
                pm->setDirection(pm->getDesiredDirection());
            }
        }
    }

    // --- COINS ---
    for (auto& coin : coins) {
        if (coin->collected) continue;

        double dx = pm->getX() - coin->getX();
        double dy = pm->getY() - coin->getY();
        double distSq = dx*dx + dy*dy;

        bool collidedByRadius = (distSq < 0.25);
        bool collidedByAABB = (std::abs(coin->getX() - pm->getX()) < 0.4 &&
                               std::abs(coin->getY() - pm->getY()) < 0.4);

        if (collidedByRadius || collidedByAABB) {
            coin->collect(); // notify de observers

            score->coinCollected(timeSinceLastCoin); // score updaten
            timeSinceLastCoin = 0.0;
        }
    }

    // --- LEVEL CLEAR ---
    bool allCollected = true; // kijken of alle coins collected zijn
    for (auto& coin : coins)
        if (!coin->collected)
            allCollected = false;
    for (auto& fruit : fruits)
        if (!fruit->collected)
            allCollected = false;

    if (allCollected) {
        score->levelCleared();  // bonuspunten voor level
        advanceLevel(); // volgend level
        return;
    }

    // --- FRUITS ---
    for (auto& fruit : fruits) {
        if (fruit->collected) continue;

        double dx = pm->getX() - fruit->getX();
        double dy = pm->getY() - fruit->getY();
        double distSq = dx*dx + dy*dy;

        bool collidedByRadius = (distSq < 0.25); // ~ radius 0.5
        bool collidedByAABB = (std::abs(fruit->getX() - pm->getX()) < 0.4 &&
                               std::abs(fruit->getY() - pm->getY()) < 0.4);

        if (collidedByRadius || collidedByAABB) {
            fruit->collect();
            score->fruitCollected(); // update score

            // Zet alle ghosts in fear mode
            for (auto& ghost : ghosts) {
                if (ghost->getMode() == GhostModel::Mode::Chase) {
                    ghost->setMode(GhostModel::Mode::Fear);
                }
            }
        }
    }

    // --- GHOSTS ---
    for (auto& ghost : ghosts) {
        ghost->update(dt);

        double dx = pm->getX() - ghost->getX();
        double dy = pm->getY() - ghost->getY();

        if (dx*dx + dy*dy < 0.50) {
            if (ghost->getMode() == GhostModel::Mode::Fear) {
                ghost->setMode(GhostModel::Mode::Eaten);
                score->ghostEaten();
            }
            else if (ghost->getMode() == GhostModel::Mode::Chase) { // opgegeten worden
                // pacman begint met death animatie
                startDeathAnimatie();
                break; // Stop checking andere ghosts
            }
        }
    }
}

// --- helper: uniforme collision / door-check ------------------------------------------------
bool World::isBlockedAt(double x, double y, double radius, bool disallowDoor) const {
    // Controleer vier hoeken rond (x,y) met gegeven radius
    if (isWallAt(x - radius, y - radius) ||
        isWallAt(x - radius, y + radius) ||
        isWallAt(x + radius, y - radius) ||
        isWallAt(x + radius, y + radius)) {
        return true;
    }

    if (disallowDoor) {
        if (isGhostDoor(x - radius, y - radius) ||
            isGhostDoor(x - radius, y + radius) ||
            isGhostDoor(x + radius, y - radius) ||
            isGhostDoor(x + radius, y + radius)) {
            return true;
        }
    }

    return false;
}

// --- vervang tryMoveEntity door deze kleine wrapper die helper gebruikt ---------------------
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

    // Pac-Man uses a larger collision radius and ghost-doors should block him.
    const double radius = 0.49;

    if (isBlockedAt(nx, ny, radius, /*disallowDoor=*/true)) {
        return;
    }

    e->setPosition(nx, ny);
}

// --- vervang canMoveIn door deze wrapper -----------------------------------------------
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

    // Pac-Man collision radius and ghostdoor should block him (disallowDoor = true)
    const double radius = 0.49;
    return !isBlockedAt(nx, ny, radius, /*disallowDoor=*/true);
}

// --- vervang tryMoveGhost door gelijksoortige wrapper -----------------------------------
void World::tryMoveGhost(std::shared_ptr<Entity> e, Direction dir, double dt) {
    if (!e || dir == Direction::NONE) return;

    double step = e->getSpeed() * dt;
    double nx = e->getX(), ny = e->getY();

    switch (dir) {
        case Direction::UP:    ny -= step; break;
        case Direction::DOWN:  ny += step; break;
        case Direction::LEFT:  nx -= step; break;
        case Direction::RIGHT: nx += step; break;
        default: break;
    }

    // Ghosts use a smaller radius and ghostdoors should NOT block them here (disallowDoor = false)
    const double radius = 0.35;

    if (isBlockedAt(nx, ny, radius, /*disallowDoor=*/false)) {
        return;
    }

    e->setPosition(nx, ny);
}

// --- vervang canGhostMove door deze kleine wrapper --------------------------------------
bool World::canGhostMove(Direction dir, double x, double y) const {
    if (dir == Direction::NONE) return false;

    double step = 0.05;
    double nx = x, ny = y;

    switch (dir) {
        case Direction::UP:    ny -= step; break;
        case Direction::DOWN:  ny += step; break;
        case Direction::LEFT:  nx -= step; break;
        case Direction::RIGHT: nx += step; break;
        default: break;
    }

    const double radius = 0.35;
    // Ghosts ignore ghostdoor tiles for movement checks here (disallowDoor = false)
    return !isBlockedAt(nx, ny, radius, /*disallowDoor=*/false);
}


std::vector<Direction> World::getFreeDirections(double x, double y) const {
    std::vector<Direction> dirs;

    // ✅ Check gewoon de aangrenzende TILES, niet de beweging zelf
    int gridX = static_cast<int>(std::floor(x));
    int gridY = static_cast<int>(std::floor(y));

    // Check of de aangrenzende tile vrij is
    if (gridY > 0 && maze[gridY - 1][gridX] != 1 && maze[gridY - 1][gridX] != 6)
        dirs.push_back(Direction::UP);

    if (gridY < mazeHeight - 1 && maze[gridY + 1][gridX] != 1 && maze[gridY + 1][gridX] != 6)
        dirs.push_back(Direction::DOWN);

    if (gridX > 0 && maze[gridY][gridX - 1] != 1 && maze[gridY][gridX - 1] != 6)
        dirs.push_back(Direction::LEFT);

    if (gridX < mazeWidth - 1 && maze[gridY][gridX + 1] != 1 && maze[gridY][gridX + 1] != 6)
        dirs.push_back(Direction::RIGHT);

    return dirs;
}



// Predict next position after moving one step in direction d
std::pair<double, double> World::predictStep(double x, double y, Direction d) const {
    double nx = x, ny = y;
    const double STEP = 0.5;

    switch (d) {
        case Direction::UP:    ny -= STEP; break;
        case Direction::DOWN:  ny += STEP; break;
        case Direction::LEFT:  nx -= STEP; break;
        case Direction::RIGHT: nx += STEP; break;
        default: break;
    }
    return {nx, ny};
}


bool World::isWallAt(double x, double y) const {
    int cx = static_cast<int>(x);
    int cy = static_cast<int>(y);

    if (cx < 0 || cy < 0 || cx >= mazeWidth || cy >= mazeHeight)
        return true;

    return maze[cy][cx] == 1;
}

bool World::isGhostDoor(double x, double y) const {
    int cx = static_cast<int>(x);
    int cy = static_cast<int>(y);

    if (cx < 0 || cy < 0 || cx >= mazeWidth || cy >= mazeHeight)
        return true;

    return maze[cy][cx] == 6;
}

bool World::isAlignedWithGrid(double x, double y) const {
    const double epsilon = 0.1; // ruimere marge

    double fractX = x - std::floor(x);
    double fractY = y - std::floor(y);

    // Check of we dicht bij 0.5 zijn (center van tile)
    return std::abs(fractX - 0.5) < epsilon &&
           std::abs(fractY - 0.5) < epsilon;
}


bool World::isIntersection(double x, double y) const {
    if (!isAlignedWithGrid(x, y)) return false;
    return getFreeDirections(x, y).size() >= 3;
}


void World::resetPositions() {
    // Reset Pac-Man
    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 4) {
                pacman->setPosition(x + 0.5, y + 0.5);
                pacman->setDirection(Direction::NONE);
                pacman->setDesiredDirection(Direction::NONE);
            }
        }
    }

    // Reset ghosts
    for (auto& ghost : ghosts) {
        ghost->setPosition(ghost->getStartX(), ghost->getStartY());
        ghost->setMode(GhostModel::Mode::Waiting);
        ghost->setReleaseTimer(0);

        if (ghost->getGhostType() == GhostModel::GhostType::AheadOfPacman2 ||
            ghost->getGhostType() == GhostModel::GhostType::DirectChase) {
            ghost->setDirection(Direction::LEFT);
            } else {
                ghost->setDirection(Direction::UP);
            }
    }
}

void World::advanceLevel() {
    currentLevel++;

    // Reset posities Pac-Man en ghosts
    resetPositions();

    // Respawn coins & fruits
    for (auto& coin : coins) coin->reset();
    for (auto& fruit : fruits) fruit->reset();

    // Moeilijkheid aanpassen
    double speedMultiplier = 1.0 + currentLevel * 0.1;   // +10% per level
    double fearMultiplier = std::max(0.3, 1.0 - currentLevel * 0.1); // fear korter, min 0.3

    for (auto& ghost : ghosts) {
        ghost->setChaseSpeed(ghost->getChaseSpeed() * speedMultiplier);
        ghost->setFearDuration(ghost->getFearDuration() * fearMultiplier);
    }
}

std::vector<Direction> World::findPath(int sx, int sy, int tx, int ty, bool allowDoor) const {
    std::vector<Direction> empty;
    if (sx == tx && sy == ty) return empty;

    if (sx < 0 || sy < 0 || sx >= mazeWidth || sy >= mazeHeight) return empty;
    if (tx < 0 || ty < 0 || tx >= mazeWidth || ty >= mazeHeight) return empty;

    auto passable = [&](int x, int y) -> bool {
        if (x < 0 || y < 0 || x >= mazeWidth || y >= mazeHeight) return false;
        if (maze[y][x] == 1) return false; // muur
        if (!allowDoor && maze[y][x] == 6) return false; // ghostdoor niet toegestaan tenzij expliciet
        return true;
    };

    int W = mazeWidth;
    int H = mazeHeight;
    std::vector<int> parent(W * H, -1);
    std::queue<std::pair<int,int>> q;
    std::vector<std::vector<bool>> vis(H, std::vector<bool>(W,false));

    q.push({sx, sy});
    vis[sy][sx] = true;

    const int dx[4] = {0, 0, -1, 1};
    const int dy[4] = {-1, 1, 0, 0};
    const Direction dirMap[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    bool found = false;
    while (!q.empty()) {
        auto [cx, cy] = q.front(); q.pop();
        if (cx == tx && cy == ty) { found = true; break; }
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || ny < 0 || nx >= W || ny >= H) continue;
            if (vis[ny][nx]) continue;
            if (!passable(nx, ny)) continue;
            vis[ny][nx] = true;
            parent[ny * W + nx] = cy * W + cx; // index van parent
            q.push({nx, ny});
        }
    }

    if (!found) return empty;

    // reconstruct path from target back to start as tiles, then convert to Directions
    std::vector<std::pair<int,int>> revTiles;
    int idx = ty * W + tx;
    while (idx != -1) {
        int px = idx % W;
        int py = idx / W;
        revTiles.push_back({px, py});
        if (px == sx && py == sy) break;
        idx = parent[idx];
    }
    if (revTiles.empty()) return empty;

    std::reverse(revTiles.begin(), revTiles.end());
    // convert neighbouring tile pairs into directions
    std::vector<Direction> path;
    for (size_t i = 1; i < revTiles.size(); ++i) {
        int px = revTiles[i-1].first;
        int py = revTiles[i-1].second;
        int cx = revTiles[i].first;
        int cy = revTiles[i].second;
        if (cx == px + 1 && cy == py) path.push_back(Direction::RIGHT);
        else if (cx == px - 1 && cy == py) path.push_back(Direction::LEFT);
        else if (cx == px && cy == py + 1) path.push_back(Direction::DOWN);
        else if (cx == px && cy == py - 1) path.push_back(Direction::UP);
        else {
            // unexpected; skip
        }
    }
    return path;
}

void World::startDeathAnimatie() {
    if (deathAnimation) return;
    deathAnimation = true;
    deathTimer = 0.0;

    // ask PacManModel how long the animation should last
    deathDuration = pacman->getDeathAnimationDuration();

    // freeze all ghosts
    for (auto& g : ghosts) g->setFrozen(true);

    // notify views via model that pacman died (PacmanView will start its death animation via observer)
    pacman->die();
}