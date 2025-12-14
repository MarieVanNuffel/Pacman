//
// Created by Marie Van Nuffel on 14/12/2025.
//

#ifndef PACMANGAME_SUBJECT_H
#define PACMANGAME_SUBJECT_H


#pragma once
#include <algorithm>
#include <vector>
#include "Observer.h"

class Subject {
public:
    void addObserver(Observer* o) { observers.push_back(o); }
    void removeObserver(Observer* o) {
        observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
    }
    void notify(int event) {
        for (auto* o : observers) {
            if(o) o->onNotify(event);
        }
    }

private:
    std::vector<Observer*> observers;
};




#endif //PACMANGAME_SUBJECT_H