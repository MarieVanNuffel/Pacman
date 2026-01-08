//
// Created by Marie Van Nuffel on 4/01/2026.
//

#ifndef PACMANGAME_EVENTS_H
#define PACMANGAME_EVENTS_H

#pragma once

namespace logic {

/**
 * @brief Event codes gebruikt door notify(int).
 *
 * Gebruik deze enum via static_cast<int>(logic::Event::NAME)
 */
enum class Event {
  COIN_RESPAWN = 0,
  COIN_COLLECTED = 1,
  PACMAN_DIED = 2,
  PACMAN_REVIVED = 3,
  FRUIT_EATEN = 4,
  FRUIT_RESPAWN = 5,
};

} // namespace logic

#endif // PACMANGAME_EVENTS_H