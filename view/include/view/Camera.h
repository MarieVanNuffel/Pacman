//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_CAMERA_H
#define PACMANGAME_CAMERA_H

#pragma once

#include <SFML/Graphics.hpp>

namespace view {

/**
 * @brief Camera voor world naar pixels om te zetten
 *
 * Zet maze-coördinaten om naar pixelcoördinaten en centreert de maze in het
 * venster.
 */
class Camera {
public:
  /**
   * @brief Constructor
   * @param pixelWidth Breedte van window in pixels
   * @param pixelHeight Hoogte van window in pixels
   * @param mazeW Breedte van de maze (tiles)
   * @param mazeH Hoogte van de maze (tiles)
   */
  Camera(int pixelWidth, int pixelHeight, int mazeW, int mazeH);

  /**
   * @brief Converteert world coördinaten naar pixel rectangle.
   *
   * @param x X-positie in world units
   * @param y Y-positie in world units
   * @param w Breedte in world units
   * @param h Hoogte in world units
   * @return SFML Rect in pixelcoördinaten
   */
  sf::FloatRect worldToPixels(double x, double y, double w, double h) const;

  /**
   * @brief Converteert tile coördinaten naar genormalizeerd [-1, 1].
   *
   * @param tileX X tile positie
   * @param tileY Y tile positie
   * @return pair (nx, ny) in [-1,1]
   */
  std::pair<double, double> tileToNormalized(double tileX, double tileY) const;

  /**
   * @brief Converteert genormalizeerde coords terug naar tile coördinaten.
   *
   * @param nx genormalizeerde X in [-1,1]
   * @param ny genormalizeerde Y in [-1,1]
   * @return pair (tileX, tileY) (centered, fractional)
   */
  std::pair<double, double> normalizedToTile(double nx, double ny) const;

  /**
   * @brief Converteert genormalizeerde center coords naar pixels.
   *
   * @param nx genormalizeerde center X in [-1,1]
   * @param ny genormalizeerde center Y in [-1,1]
   * @param wFrac breedte als fractie van de totale maze breedte
   * @param hFrac hoogte als fractie van de totale maze hoogte
   * @return sf::FloatRect (top-left x,y en width,height in pixels)
   */
  sf::FloatRect normalizedToPixels(double nx, double ny, double wFrac,
                                   double hFrac) const;

  int mazeWidth;
  int mazeHeight;

private:
  int wPixels;
  int hPixels;

  float cellSize;
  float offsetX;
  float offsetY;

  /**
   * @brief Herberekent cell size en offsets
   */
  void recompute();
};
} // namespace view

#endif // PACMANGAME_CAMERA_H