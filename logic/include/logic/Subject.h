//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_SUBJECT_H
#define PACMANGAME_SUBJECT_H


#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "Observer.h"

class Subject {
public:
    // Voeg een observer toe (geef een shared_ptr<Observer> door).
    void addObserver(std::shared_ptr<Observer> o);

    // Verwijder een observer (geef dezelfde shared_ptr door).
    void removeObserver(std::shared_ptr<Observer> o);

    // Notificeer alle levende observers en verwijder verlopen weak_ptrs
    void notify(int event);

private:
    std::vector<std::weak_ptr<Observer>> observers;
};




#endif //PACMANGAME_SUBJECT_H