//
// Created by Marie Van Nuffel on 28/11/2025.
//

#include "../include/view/Camera.h"

namespace view {
Camera::Camera(int pixelW, int pixelH, int mazeW, int mazeH)
    : mazeWidth(mazeW), mazeHeight(mazeH), wPixels(pixelW), hPixels(pixelH) {
  recompute();
}

sf::FloatRect Camera::worldToPixels(double x, double y, double w,
                                    double h) const {
  // converteren naar pixels + naar midden verschuiven
  float px = offsetX + static_cast<float>(x) * cellSize;
  float py = offsetY + static_cast<float>(y) * cellSize;
  float pw = static_cast<float>(w) * cellSize;
  float ph = static_cast<float>(h) * cellSize;

  return {px, py, pw, ph};
}

std::pair<double, double> Camera::tileToNormalized(double tileX,
                                                   double tileY) const {
  // tileX in [0..mazeWidth], tile center bij x = tileIndex + 0.5
  double nx = (tileX / static_cast<double>(mazeWidth)) * 2.0 - 1.0;
  double ny = (tileY / static_cast<double>(mazeHeight)) * 2.0 - 1.0;
  return {nx, ny};
}

std::pair<double, double> Camera::normalizedToTile(double nx, double ny) const {
  double tx = ((nx + 1.0) / 2.0) * static_cast<double>(mazeWidth);
  double ty = ((ny + 1.0) / 2.0) * static_cast<double>(mazeHeight);
  return {tx, ty};
}

sf::FloatRect Camera::normalizedToPixels(double nx, double ny, double wFrac,
                                         double hFrac) const {
  // total pixel-size van de maze zelf (niet het hele venster)
  float totalW = cellSize * static_cast<float>(mazeWidth);
  float totalH = cellSize * static_cast<float>(mazeHeight);

  // center pixelpositie van de normalized coord binnen de maze area
  float centerPx = offsetX + static_cast<float>((nx + 1.0) * 0.5) * totalW;
  float centerPy = offsetY + static_cast<float>((ny + 1.0) * 0.5) * totalH;

  // breedte en hoogte als fractie van de maze (wFrac is een deel van [0,1])
  float pw = static_cast<float>(wFrac) * totalW;
  float ph = static_cast<float>(hFrac) * totalH;

  // convert center -> top-left
  float left = centerPx - pw * 0.5f;
  float top = centerPy - ph * 0.5f;

  return {left, top, pw, ph};
}

void Camera::recompute() {
  // berekent cellsize per as
  float cellW = static_cast<float>(wPixels) / std::max(1, mazeWidth);  // horizontaal
  float cellH = static_cast<float>(hPixels) / std::max(1, mazeHeight); // verticaal

  cellSize = std::min(cellW, cellH); // kleinste nemen

  float totalW = cellSize * mazeWidth;
  float totalH = cellSize * mazeHeight;

  // centreren
  offsetX = (wPixels - totalW) / 2.0f;
  offsetY = (hPixels - totalH) / 2.0f;
}
} // namespace view