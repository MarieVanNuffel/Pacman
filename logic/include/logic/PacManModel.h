//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_PACMANMODEL_H
#define PACMANGAME_PACMANMODEL_H


#pragma once

#include "Entity.h"
#include "Direction.h"
#include "Subject.h"

namespace logic {

/**
 * @brief Model voor de pacman entity
 *
 * Houdt lives bij, direction en death animatie timing.
 * Movement gebeurt in World.
 */
class PacManModel : public Entity, public Subject {
private:
  int lives = 3;
  double deathAnimationDuration = 12 * 0.15; // 12 frames * 0.15s

public:
  PacManModel();

  // Setters
  /**
   * @brief Zet huidige movement direction
   * @param d Nieuwe direction
   */
  void setDirection(Direction d) override;

  /**
   * @brief Zet Desired direction
   * @param d Desired direction
   */
  void setDesiredDirection(Direction d) { desiredDirection = d; }

  // Getters
  /**
   * @brief Geeft aantal levens terug
   * @return Aantal lives
   */
  [[nodiscard]] int getLives() const { return lives; }

  /**
   * @brief Geeft duur van death animatie terug
   * @return Death animation duration
   */
  [[nodiscard]] double getDeathAnimationDuration() const {
    return deathAnimationDuration;
  }

  // pacman leven
  /**
   * @brief Verlaag lives met 1
   */
  void loseLife() { --lives; };

  /**
   * @brief Check of game over is
   * @return true als lives <= 0
   */
  bool isGameOver() const { return lives <= 0; }

  /**
   * @brief Notify observers dat pacman doodgaat
   */
  void die();

  /**
   * @brief Notify observers dat pacman opnieuw leeft
   */
  void revive();

  /**
   * @brief Reset pacman naar start state
   */
  void reset();

  /**
   * @brief Update functie
   *
   * Movement gebeurt in World.
   * @param dt Delta time
   */
  void update(double dt) override;
};
} // namespace logic

#endif // PACMANGAME_PACMANMODEL_H