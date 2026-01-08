//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_STOPWATCH_H
#define PACMAN_STOPWATCH_H

#pragma once
#include <chrono>

namespace logic {

/**
 * @brief Singleton stopwatch voor delta-time berekening
 *
 * Kan gebruikt worden om frame-timing bij te houden.
 */
class Stopwatch {
public:
  /**
   * @brief Verkrijg singleton instantie
   * @return Referentie naar Stopwatch
   */
  static Stopwatch &instance();

  /**
   * @brief Bereken tijd sinds laatste tick
   * @return Delta-time in seconden
   */
  double tick();

private:
  /**
   * @brief Constructor, initieert starttijd
   */
  Stopwatch();
  std::chrono::steady_clock::time_point last;
};
} // namespace logic

#endif // PACMAN_STOPWATCH_H