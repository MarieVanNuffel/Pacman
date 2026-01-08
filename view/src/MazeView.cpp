//
// Created by Marie Van Nuffel on 27/11/2025.
//
#include "../include/view/MazeView.h"

namespace view {
MazeView::MazeView(const logic::World *w) : world(w) {}

void MazeView::draw(sf::RenderWindow &win, const Camera &cam) {
  const auto &maze = world->getMaze(); // vector met getallen
  int H = maze.size();
  int W = maze[0].size();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {

      double sizeW = 1.0; // 1 cel breed
      double sizeH = 1.0; // 1 cel hoog

      // tile center
      double tileCenterX = x + 0.5;
      double tileCenterY = y + 0.5;

      auto [nx, ny] = cam.tileToNormalized(tileCenterX, tileCenterY);
      double wFrac = sizeW / static_cast<double>(cam.mazeWidth);
      double hFrac = sizeH / static_cast<double>(cam.mazeHeight);

      sf::FloatRect rect = cam.normalizedToPixels(nx, ny, wFrac, hFrac);

      // tekenen met SFML
      sf::RectangleShape tile;
      tile.setPosition(rect.left, rect.top);
      tile.setSize(sf::Vector2f(rect.width, rect.height));

      if (maze[y][x] == 1) {
        tile.setFillColor(sf::Color(0, 0, 150)); // muren blauw
      } else {
        tile.setFillColor(sf::Color(0, 0, 0)); // leeg = zwart
      }

      win.draw(tile);
    }
  }
}
} // namespace view