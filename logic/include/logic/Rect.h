//
// Created by Marie Van Nuffel on 28/11/2025.
//

#ifndef PACMANGAME_RECT_H
#define PACMANGAME_RECT_H

#pragma once

namespace logic {

/**
 * @brief Axis-aligned rectangle
 *
 * Wordt gebruikt voor collision detection.
 */
struct Rect {
  double x;
  double y;
  double w;
  double h;

  /**
   * @brief Check of twee rectangles overlappen
   * @param other Andere rectangle
   * @return True als ze kruisen
   */
  bool intersects(const Rect &other) const {
    return !(x + w / 2 < other.x - other.w / 2 ||
             other.x + other.w / 2 < x - w / 2 ||
             y + h / 2 < other.y - other.h / 2 ||
             other.y + other.h / 2 < y - h / 2);
  }
};
} // namespace logic

#endif // PACMANGAME_RECT_H