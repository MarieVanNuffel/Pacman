//
// Created by Marie Van Nuffel on 14/12/2025.
//

#include "../include/logic/Subject.h"
#include <algorithm>

namespace logic {
    void Subject::addObserver(std::shared_ptr<Observer> o) {
        if (!o) return;
        // voorkom dubbele registratie (optioneel)
        for (auto &w : observers) {
            if (auto s = w.lock()) {
                if (s == o) return;
            }
        }
        observers.emplace_back(o);
    }

    void Subject::removeObserver(std::shared_ptr<Observer> o) {
        observers.erase(std::remove_if(observers.begin(), observers.end(),
            [&](const std::weak_ptr<Observer>& w) {
                auto s = w.lock();
                return (!s) || (o && s == o);
            }), observers.end());
    }

    void Subject::notify(int event) {
        for (auto it = observers.begin(); it != observers.end(); ) {
            if (auto s = it->lock()) {
                s->onNotify(event);
                ++it;
            } else {
                it = observers.erase(it);
            }
        }
    }
}
