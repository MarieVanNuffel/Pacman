//
// Created by Marie Van Nuffel on 27/11/2025.
//

#include "logic/World.h"
#include "logic/Random.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>

namespace logic {
World::World(const std::shared_ptr<IEntityFactory> &factory_,
             const std::shared_ptr<Score> &score_)
    : factory(factory_), score(score_) {
  maze = {{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
           {1, 3, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1},
           {1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
           {1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
           {1, 2, 1, 2, 1, 1, 2, 1, 1, 6, 6, 1, 1, 2, 1, 1, 2, 1, 2, 1},
           {1, 2, 2, 2, 2, 2, 2, 1, 5, 5, 5, 5, 1, 2, 2, 2, 2, 2, 2, 1},
           {1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1},
           {1, 2, 1, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
           {1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
           {1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 3, 1},
           {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}

  };
  mazeHeight = maze.size();
  mazeWidth = maze[0].size();
}

void World::spawnEntitiesForLevel() {
  // --- PACMAN ---
  pacman = std::make_shared<PacManModel>();
  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 4) { // 4 = pacman
        pacman->setPosition(x + 0.5, y + 0.5);
      }
    }
  }

  // --- GHOSTS ---
  ghosts.clear();

  int ghostIndex = 0;

  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 5) { // 5 = ghost
        // Bepaal type ghost op basis van spawn volgorde
        GhostModel::GhostType type;
        switch (ghostIndex) {
        case 0:
          type = GhostModel::GhostType::AheadOfPacman1;
          break;
        case 1:
          type = GhostModel::GhostType::LockedRandom;
          break;
        case 2:
          type = GhostModel::GhostType::AheadOfPacman2;
          break;
        case 3:
          type = GhostModel::GhostType::DirectChase;
          break;
        default:
          type = GhostModel::GhostType::LockedRandom;
          break;
        }

        auto ghost = std::make_shared<GhostModel>(type);
        ghost->setStartPosition(x + 0.5, y + 0.5);
        ghost->setWorld(shared_from_this()); // belangrijk voor AI

        ghosts.push_back(ghost);
        ghostIndex++;
      }
    }
  }

  // --- COINS ---
  coins.clear();

  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 2) { // coin = 2
        auto c = std::make_shared<CoinModel>();
        c->setPosition(x + 0.5, y + 0.5);
        coins.push_back(c);
      }
    }
  }

  // --- FRUITS ---
  fruits.clear();

  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 3) { // fruit = 3
        auto f = std::make_shared<FruitModel>();
        f->setPosition(x + 0.5, y + 0.5);
        fruits.push_back(f);
      }
    }
  }

  // --- GHOSTDOOR ---
  ghostDoors.clear();

  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 6) { // ghostdoor = 6
        auto door = std::make_shared<GhostDoorModel>();
        door->setPosition(x + 0.5, y + 0.5);
        ghostDoors.push_back(door);
      }
    }
  }
}

void World::advanceLevel() {
  currentLevel++;

  // Reset posities pacman en ghosts
  resetPositions();

  // Respawn coins & fruits
  for (auto &coin : coins)
    coin->reset();
  for (auto &fruit : fruits)
    fruit->reset();

  // Moeilijkheid aanpassen
  double speedMultiplier = 1.0 + currentLevel * 0.1; // +10% sneller per level
  double fearMultiplier =
      std::max(0.3, 1.0 - currentLevel * 0.1); // fear mode korter

  for (auto &ghost : ghosts) {
    ghost->setChaseSpeed(ghost->getChaseSpeed() * speedMultiplier);
    ghost->setFearDuration(ghost->getFearDuration() * fearMultiplier);
  }
}

void World::resetPositions() const {
  // Reset pacman
  for (size_t y = 0; y < maze.size(); ++y) {
    for (size_t x = 0; x < maze[y].size(); ++x) {
      if (maze[y][x] == 4) {
        pacman->setPosition(x + 0.5, y + 0.5);
        pacman->setDirection(Direction::RIGHT); // naar rechts laten beginnen
        pacman->setDesiredDirection(Direction::RIGHT);
      }
    }
  }

  // Reset ghosts
  for (auto &ghost : ghosts) {
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

void World::startDeathAnimatie() {
  if (deathAnimation)
    return;
  deathAnimation = true;
  deathTimer = 0.0;

  deathDuration = pacman->getDeathAnimationDuration();

  // freeze alle ghosts
  for (auto &g : ghosts)
    g->setFrozen(true);

  // notify views via observer
  pacman->die();
}

void World::update(double dt) {
  auto pm = pacman;
  if (!pm)
    return;

  if (deathAnimation) {
    deathTimer += dt;
    if (deathTimer >= deathDuration) {
      pacman->loseLife(); // - leven pacman
      resetPositions();   // terug naar startposities

      pacman->revive(); // notify de observers

      for (auto &g : ghosts)
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
      if (nearCenter &&
          canMoveIn(pm->getDesiredDirection(), gridCenterX,
                    gridCenterY)) { // is het mogelijk vanaf tile center
        // snap naar center en zet direction naar DesiredDirection
        pm->setPosition(gridCenterX, gridCenterY);
        pm->setDirection(pm->getDesiredDirection());
      }
    }
  }

  // 2) Corner snapping bij tile centers
  if (pm->getDirection() == Direction::UP ||
      pm->getDirection() == Direction::DOWN) {
    if (std::abs(pm->getX() - gridCenterX) < snapEpsilon)
      pm->setPosition(gridCenterX, pm->getY());
  } else if (pm->getDirection() == Direction::LEFT ||
             pm->getDirection() == Direction::RIGHT) {
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
      if (nearCenter &&
          canMoveIn(pm->getDesiredDirection(), gridCenterX, gridCenterY)) {
        pm->setPosition(gridCenterX, gridCenterY);
        pm->setDirection(pm->getDesiredDirection());
      }
    }
  }

  // --- COINS ---
  for (auto &coin : coins) {
    if (coin->collected)
      continue;

    double dx = pm->getX() - coin->getX();
    double dy = pm->getY() - coin->getY();
    double distSq = dx * dx + dy * dy;

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
  for (auto &coin : coins)
    if (!coin->collected)
      allCollected = false;
  for (auto &fruit : fruits)
    if (!fruit->collected)
      allCollected = false;

  if (allCollected) {
    score->levelCleared(); // bonuspunten voor level
    advanceLevel();        // volgend level
    return;
  }

  // --- FRUITS ---
  for (auto &fruit : fruits) {
    if (fruit->collected)
      continue;

    double dx = pm->getX() - fruit->getX();
    double dy = pm->getY() - fruit->getY();
    double distSq = dx * dx + dy * dy;

    bool collidedByRadius = (distSq < 0.25); // ~ radius 0.5
    bool collidedByAABB = (std::abs(fruit->getX() - pm->getX()) < 0.4 &&
                           std::abs(fruit->getY() - pm->getY()) < 0.4);

    if (collidedByRadius || collidedByAABB) {
      fruit->collect();
      score->fruitCollected(); // update score

      // Zet alle ghosts in fear mode
      for (auto &ghost : ghosts) {
        if (ghost->getMode() == GhostModel::Mode::Chase) {
          ghost->setMode(GhostModel::Mode::Fear);
        }
      }
    }
  }

  // --- GHOSTS ---
  for (auto &ghost : ghosts) {
    ghost->update(dt);

    double dx = pm->getX() - ghost->getX();
    double dy = pm->getY() - ghost->getY();

    if (dx * dx + dy * dy < 0.50) {
      if (ghost->getMode() == GhostModel::Mode::Fear) {
        ghost->setMode(GhostModel::Mode::Eaten);
        score->ghostEaten();
      } else if (ghost->getMode() ==
                 GhostModel::Mode::Chase) { // opgegeten worden
        // pacman begint met death animatie
        startDeathAnimatie();
        break; // Stop checking andere ghosts
      }
    }
  }
}

// Collision helpers
bool World::isWallAt(double x, double y) const {
  int cx = static_cast<int>(x);
  int cy = static_cast<int>(y);

  if (cx < 0 || cy < 0 || cx >= mazeWidth || cy >= mazeHeight) // buiten de map
    return true;

  return maze[cy][cx] == 1; // 1 = muur
}

bool World::isBlockedAt(double x, double y, double radius,
                        bool disallowDoor) const {
  // Controleer vier hoeken rond (x,y) met gegeven collision radius
  if (isWallAt(x - radius, y - radius) || isWallAt(x - radius, y + radius) ||
      isWallAt(x + radius, y - radius) || isWallAt(x + radius, y + radius)) {
    return true;
  }

  if (disallowDoor) { // als er niet door de deur gegaan mag worden, wordt het
                      // gezien als muur
    if (isGhostDoor(x - radius, y - radius) ||
        isGhostDoor(x - radius, y + radius) ||
        isGhostDoor(x + radius, y - radius) ||
        isGhostDoor(x + radius, y + radius)) {
      return true;
    }
  }
  return false;
}

bool World::isGhostDoor(double x, double y) const {
  int cx = static_cast<int>(x);
  int cy = static_cast<int>(y);

  if (cx < 0 || cy < 0 || cx >= mazeWidth || cy >= mazeHeight)
    return true;

  return maze[cy][cx] == 6; // 6 = ghostdoor
}

// Grid helpers
bool World::isAlignedWithGrid(double x, double y) {
  constexpr double epsilon = 0.1; // marge

  double fractX = x - std::floor(x);
  double fractY = y - std::floor(y);

  // Check of we dicht bij 0.5 zijn (center van tile)
  return std::abs(fractX - 0.5) < epsilon && std::abs(fractY - 0.5) < epsilon;
}

bool World::isIntersection(double x, double y) const {
  if (!isAlignedWithGrid(x, y))
    return false;
  return getFreeDirections(x, y).size() >= 3;
}

std::vector<Direction> World::getFreeDirections(double x, double y) const {
  std::vector<Direction> dirs;

  // zet om naar grid coördinaten
  int gridX = static_cast<int>(std::floor(x));
  int gridY = static_cast<int>(std::floor(y));

  // Check of de aangrenzende tile vrij is in een bepaalde richting en voegt die
  // richting dan toe aan de vector
  if (gridY > 0 && maze[gridY - 1][gridX] != 1 && maze[gridY - 1][gridX] != 6)
    dirs.push_back(Direction::UP);

  if (gridY < mazeHeight - 1 && maze[gridY + 1][gridX] != 1 &&
      maze[gridY + 1][gridX] != 6)
    dirs.push_back(Direction::DOWN);

  if (gridX > 0 && maze[gridY][gridX - 1] != 1 && maze[gridY][gridX - 1] != 6)
    dirs.push_back(Direction::LEFT);

  if (gridX < mazeWidth - 1 && maze[gridY][gridX + 1] != 1 &&
      maze[gridY][gridX + 1] != 6)
    dirs.push_back(Direction::RIGHT);

  return dirs;
}

std::pair<double, double> World::predictStep(double x, double y,
                                             Direction d) {
  double nx = x, ny = y;
  constexpr double STEP = 0.5;

  switch (d) { // afhankelijk van de richting, pas het x of y coördinaat aan met
               // + of -
  case Direction::UP:
    ny -= STEP;
    break;
  case Direction::DOWN:
    ny += STEP;
    break;
  case Direction::LEFT:
    nx -= STEP;
    break;
  case Direction::RIGHT:
    nx += STEP;
    break;
  default:
    break;
  }
  return {nx, ny};
}

// Movement helpers
void World::tryMoveEntity(const std::shared_ptr<Entity> &e, Direction dir, double dt) const {
  if (!e || dir == Direction::NONE)
    return;

  double step = e->getSpeed() * dt;
  double nx = e->getX();
  double ny = e->getY();

  switch (dir) {
  case Direction::UP:
    ny -= step;
    break;
  case Direction::DOWN:
    ny += step;
    break;
  case Direction::LEFT:
    nx -= step;
    break;
  case Direction::RIGHT:
    nx += step;
    break;
  default:
    break;
  }

  constexpr double radius = 0.49; // collision radius

  if (isBlockedAt(nx, ny, radius,
                  /*disallowDoor=*/true)) { // mag niet door ghostdoor
    return;
  }
  e->setPosition(nx, ny);
}

void World::tryMoveGhost(const std::shared_ptr<Entity> &e, Direction dir, double dt) const {
  if (!e || dir == Direction::NONE)
    return;

  double step = e->getSpeed() * dt;
  double nx = e->getX(), ny = e->getY();

  switch (dir) {
  case Direction::UP:
    ny -= step;
    break;
  case Direction::DOWN:
    ny += step;
    break;
  case Direction::LEFT:
    nx -= step;
    break;
  case Direction::RIGHT:
    nx += step;
    break;
  default:
    break;
  }

  constexpr double radius = 0.35; // ghosts hebben een kleinere radius

  if (isBlockedAt(nx, ny, radius,
                  /*disallowDoor=*/false)) { // mag door ghostdoor
    return;
  }
  e->setPosition(nx, ny);
}

bool World::canMoveIn(Direction dir, double x, double y) const {
  if (dir == Direction::NONE)
    return false;

  double checkStep = 0.05; // kleine check-step
  double nx = x;
  double ny = y;

  switch (dir) {
  case Direction::UP:
    ny -= checkStep;
    break;
  case Direction::DOWN:
    ny += checkStep;
    break;
  case Direction::LEFT:
    nx -= checkStep;
    break;
  case Direction::RIGHT:
    nx += checkStep;
    break;
  default:
    break;
  }

  constexpr double radius = 0.49; // pacman radius
  return !isBlockedAt(nx, ny, radius,
                      /*disallowDoor=*/true); // niet door ghostdoor
}

bool World::canGhostMove(Direction dir, double x, double y) const {
  if (dir == Direction::NONE)
    return false;

  double step = 0.05;
  double nx = x, ny = y;

  switch (dir) {
  case Direction::UP:
    ny -= step;
    break;
  case Direction::DOWN:
    ny += step;
    break;
  case Direction::LEFT:
    nx -= step;
    break;
  case Direction::RIGHT:
    nx += step;
    break;
  default:
    break;
  }

  constexpr double radius = 0.35;
  return !isBlockedAt(nx, ny, radius,
                      /*disallowDoor=*/false); // mag door ghostdoor
}

std::vector<Direction> World::findPath(int sx, int sy, int tx, int ty,
                                       bool allowDoor) const {
  std::vector<Direction> empty;

  if (sx == tx && sy == ty)
    return empty; // Als start en target gelijk zijn, is er geen pad nodig

  // Check of de posities geldig zijn
  if (sx < 0 || sy < 0 || sx >= mazeWidth || sy >= mazeHeight)
    return empty;

  if (tx < 0 || ty < 0 || tx >= mazeWidth || ty >= mazeHeight)
    return empty;

  // kijken of het muren/ghostdoors zijn (afhankelijk van allowdoor)
  auto passable = [&](int x, int y) -> bool {
    if (x < 0 || y < 0 || x >= mazeWidth || y >= mazeHeight)
      return false;

    if (maze[y][x] == 1) // 1 = muur
      return false;

    if (!allowDoor && maze[y][x] == 6) // 6 = ghostdoor
      return false;

    return true;
  };

  int W = mazeWidth;
  int H = mazeHeight;

  // Parent array om het pad te reconstrueren
  // parent[i] bevat de index van de vorige tile
  std::vector<int> parent(W * H, -1);

  // Queue voor breadth first search
  std::queue<std::pair<int, int>> q;

  // Matrix om de al bezochte tiles bij te houden
  std::vector<std::vector<bool>> vis(H, std::vector<bool>(W, false));

  // Start breadth first vanaf start tile
  q.push({sx, sy});
  vis[sy][sx] = true;

  // Richtingen (up, down, left, right)

  bool found = false;

  while (!q.empty()) {
    auto [cx, cy] = q.front();
    q.pop();

    // Target bereikt
    if (cx == tx && cy == ty) {
      found = true;
      break;
    }

    // Overloop alle 4 richtingen
    for (int i = 0; i < 4; ++i) {
      constexpr int dy[4] = {-1, 1, 0, 0};
      constexpr int dx[4] = {0, 0, -1, 1};
      int nx = cx + dx[i];
      int ny = cy + dy[i];

      // Bounds check
      if (nx < 0 || ny < 0 || nx >= W || ny >= H)
        continue;

      // al bezocht of niet
      if (vis[ny][nx])
        continue;

      // Collision check
      if (!passable(nx, ny))
        continue;

      // Markeer als bezocht en onthoud parent
      vis[ny][nx] = true;
      parent[ny * W + nx] = cy * W + cx;

      // Voeg toe aan queue
      q.push({nx, ny});
    }
  }

  // Geen pad gevonden
  if (!found)
    return empty;

  // Bouw het pad op van target naar start
  std::vector<std::pair<int, int>> revTiles;
  int idx = ty * W + tx;

  while (idx != -1) {
    int px = idx % W;
    int py = idx / W;
    revTiles.push_back({px, py});

    // Stop wanneer we terug aan de start zijn
    if (px == sx && py == sy)
      break;

    idx = parent[idx];
  }

  if (revTiles.empty())
    return empty;

  // Draai om zodat het pad van start naar target loopt
  std::reverse(revTiles.begin(), revTiles.end());

  // Tiles worden in richtingen omgezet
  std::vector<Direction> path;

  for (size_t i = 1; i < revTiles.size(); ++i) {
    int px = revTiles[i - 1].first;
    int py = revTiles[i - 1].second;
    int cx = revTiles[i].first;
    int cy = revTiles[i].second;

    // Bepaal richting op basis van tile verschil
    if (cx == px + 1 && cy == py)
      path.push_back(Direction::RIGHT);
    else if (cx == px - 1 && cy == py)
      path.push_back(Direction::LEFT);
    else if (cx == px && cy == py + 1)
      path.push_back(Direction::DOWN);
    else if (cx == px && cy == py - 1)
      path.push_back(Direction::UP);
  }

  return path;
}
} // namespace logic