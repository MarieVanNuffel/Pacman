//
// Created by Marie Van Nuffel on 27/11/2025.
//

#ifndef PACMANGAME_GHOSTMODEL_H
#define PACMANGAME_GHOSTMODEL_H

#pragma once

#include <cassert>

#include "Entity.h"
#include "Subject.h"

namespace logic {
class World;
}

namespace logic {
/**
 * @class GhostModel
 * @brief Model voor een ghost met AI.
 *
 * Behaviors: Waiting, Chase, Fear, Eaten. Ghost heeft pointer naar World
 * (worldRef) om helpers te gebruiken.
 */
class GhostModel : public Entity, public Subject {
public:
  /**
   * @brief Mogelijke modes van ghosts.
   */
  enum class Mode { Waiting, Chase, Fear, Eaten };
  /**
   * @brief Verschillende AI types per ghost.
   */
  enum class GhostType {
    LockedRandom,
    AheadOfPacman1,
    AheadOfPacman2,
    DirectChase
  };

  /**
   * @brief Maakt een ghost met een bepaald AI type.
   *
   * @param t Type van de ghost AI.
   */
  GhostModel(GhostType t);

  /**
   * @brief Update de ghost per tick.
   *
   * Verwerkt AI beslissingen, movement en state transitions.
   *
   * @param dt Delta time in seconden.
   */
  void update(double dt) override;

  // Getters

  /**
   * @brief Geeft het ghost type terug.
   * @return GhostType van de ghost.
   */
  [[nodiscard]] Mode getMode() const { return mode; };
  /**
   * @brief Geeft het ghost type terug.
   * @return GhostType van de ghost.
   */
  [[nodiscard]] GhostType getGhostType() const { return type; }
  /**
   * @brief Geeft de start X-positie terug.
   * @return Start X-positie.
   */
  [[nodiscard]] double getStartX() const { return startX; }
  /**
   * @brief Geeft de start Y-positie terug.
   * @return Start Y-positie.
   */
  [[nodiscard]] double getStartY() const { return startY; }
  /**
   * @brief Geeft de fear duration terug.
   * @return Duur van fear mode in seconden.
   */
  [[nodiscard]] double getFearDuration() const { return fearDuration; }
  /**
   * @brief Geeft de chase snelheid terug.
   * @return Snelheid tijdens chase mode.
   */
  [[nodiscard]] double getChaseSpeed() const { return chaseSpeed; }
  /**
   * @brief Geeft terug of de ghost bevroren is.
   * @return True als de ghost frozen is.
   */
  [[nodiscard]] bool isFrozen() const { return frozen; }

  // Setters

  /**
   * @brief Zet een referentie naar de world.
   *
   * @param w Pointer naar World.
   */
  void setWorld(const std::shared_ptr<World> &w) {
    assert(w && "setWorld called with null");
    worldRef = w;
  };
  /**
   * @brief Zet de mode van de ghost.
   *
   * Past snelheid en timers aan indien nodig.
   *
   * @param m Nieuwe mode.
   */
  void setMode(Mode m);
  /**
   * @brief Zet de huidige snelheid.
   *
   * @param s Nieuwe snelheid.
   */
  void setSpeed(double s) { speed = s; }
  /**
   * @brief Zet de fear duration.
   *
   * @param d Duur in seconden.
   */
  void setFearDuration(double d) { fearDuration = d; }
  /**
   * @brief Zet de chase snelheid.
   *
   * @param d Nieuwe chase snelheid.
   */
  void setChaseSpeed(double d) { chaseSpeed = d; };
  /**
   * @brief Zet de startpositie van de ghost.
   *
   * @param sx Start X-positie.
   * @param sy Start Y-positie.
   */
  void setStartPosition(double sx, double sy) {
    x = sx;
    y = sy;
    startX = sx;
    startY = sy;
  }
  /**
   * @brief Zet of de ghost bevroren is.
   *
   * @param f True om te bevriezen.
   */
  void setFrozen(bool f) { frozen = f; }
  /**
   * @brief Zet de release timer.
   *
   * @param dt Tijd sinds start.
   */
  void setReleaseTimer(double dt) { releaseTimer = dt; }

private:
  GhostType type;
  Mode mode;

  double chaseSpeed;
  bool frozen = false;

  // Startposities
  double startX = 0.0;
  double startY = 0.0;

  double releaseTimer = 0.0;
  double decisionTimer = 0.0; // wanneer beslissingen nemen
  double decisionCooldown = 0.12;

  double fearTimer = 0.0;
  double fearDuration = 6.0;

  std::weak_ptr<World> worldRef;
};
} // namespace logic
#endif // PACMANGAME_GHOSTMODEL_H