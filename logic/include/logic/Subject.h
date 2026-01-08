//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_SUBJECT_H
#define PACMANGAME_SUBJECT_H

#pragma once

#include "Observer.h"
#include <memory>
#include <vector>

namespace logic {

/**
 * @brief Subject in het Observer-patroon
 *
 * Beheert een lijst van observers en notify bij events.
 * (Observers worden opgeslagen als weak_ptr om dangling pointers te vermijden.)
 */
class Subject {
public:
  /**
   * @brief Voeg een observer toe
   * @param o Shared pointer naar de observer die toegevoegd wordt.
   *
   */
  void addObserver(std::shared_ptr<Observer> o);

  /**
   * @brief Verwijder een observer
   * @param o Shared pointer naar de observer die verwijderd wordt.
   *
   * Verwijdert ook verlopen weak_ptrs.
   */
  void removeObserver(std::shared_ptr<Observer> o);

  /**
   * @brief Notify de observers
   * @param event Event code die wordt doorgegeven aan observers
   *
   * Verwijdert automatisch verlopen observers.
   */
  void notify(int event);

private:
  std::vector<std::weak_ptr<Observer>> observers;
};
} // namespace logic

#endif // PACMANGAME_SUBJECT_H