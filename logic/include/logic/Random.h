//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMAN_RANDOM_H
#define PACMAN_RANDOM_H

#pragma once
#include <random>

namespace logic {

/**
 * @brief Singleton random number generator
 *
 * Wordt gebruikt voor random ghost gedrag.
 */
class Random {
public:
  /**
   * @brief Geeft singleton instance terug
   * @return Referentie naar Random instance
   */
  static Random &instance();

  /**
   * @brief Genereert random int in [a, b]
   * @param a Ondergrens
   * @param b Bovengrens
   * @return Random int
   */
  int nextInt(int a, int b);

  /**
   * @brief Genereert random double in [a, b]
   * @param a Ondergrens
   * @param b Bovengrens
   * @return Random double
   */
  double nextDouble(double a, double b);

private:
  /**
   * @brief Private constructor voor singleton
   */
  Random();
  std::mt19937 rng; // Mersenne Twister RNG
};
} // namespace logic

#endif // PACMAN_RANDOM_H