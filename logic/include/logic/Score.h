//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_SCORE_H
#define PACMANGAME_SCORE_H

#pragma once

#include <string>
#include <vector>

#include "Observer.h"

namespace logic {

/**
 * @brief Beheert score en highscores
 *
 * Reageert op game events via observer.
 */
class Score : public Observer {
public:
  /**
   * @brief Constructor
   *
   * Laadt highscores.
   */
  Score();

  // Score aanpassingen

  /**
   * @brief Verwerkt coin score afhankelijk van tijd tussen collected coins
   * @param timeSinceLastCoin Tijd sinds vorige coin
   */
  void coinCollected(double timeSinceLastCoin);

  /**
   * @brief Verhoog score bij level clear
   */
  void levelCleared();

  /**
   * @brief Verhoog score bij fruit
   */
  void fruitCollected();

  /**
   * @brief Verhoog score bij ghost eat
   */
  void ghostEaten();

  /**
   * @brief Verlaag score bij pacman death
   */
  void pacmanDied();

  // Observer
  /**
   * @brief Ontvangt events van entities
   * @param event Event id
   */
  void onNotify(int event) override;

  // Highscores
  /**
   * @brief Laad highscores
   */
  void loadHighScores();

  /**
   * @brief Sla highscores op
   */
  void saveHighScores();

  // Getters

  /**
   * @brief Geef huidige score
   * @return Current score
   */
  [[nodiscard]] int getCurrentScore() const { return currentScore; }

  /**
   * @brief Geef highscores
   * @return Vector met highscores
   */
  [[nodiscard]] const std::vector<int> &getHighScores() const {
    return highScores;
  }

private:
  int currentScore = 0;
  std::vector<int> highScores;
};
} // namespace logic

#endif // PACMANGAME_SCORE_H